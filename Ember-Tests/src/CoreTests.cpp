#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <Ember/Core/UUID.h>
#include <Ember/Core/Timestep.h>
#include <Ember/Core/Timer.h>
#include <Ember/Core/LayerStack.h>
#include <Ember/Renderer/ShaderCompiler.h>

#include <thread>
#include <chrono>
#include <set>

using Catch::Approx;

TEST_CASE("UUID uniqueness", "[core][uuid]")
{
	std::set<uint64_t> ids;
	constexpr int N = 1000;
	for (int i = 0; i < N; i++)
		ids.insert(static_cast<uint64_t>(Ember::UUID()));

	REQUIRE(ids.size() == N);
}

TEST_CASE("UUID from value", "[core][uuid]")
{
	Ember::UUID id(42);
	REQUIRE(static_cast<uint64_t>(id) == 42ull);
}

TEST_CASE("Timestep conversions", "[core][timestep]")
{
	Ember::Timestep ts(0.016f);
	REQUIRE(ts.GetSeconds() == Approx(0.016f));
	REQUIRE(ts.GetMilliseconds() == Approx(16.0f));
	REQUIRE(static_cast<float>(ts) == Approx(0.016f));
}

TEST_CASE("Timer elapses", "[core][timer]")
{
	Ember::Timer timer;
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	REQUIRE(timer.ElapsedMillis() >= 15.0f);
}

TEST_CASE("LayerStack push pop", "[core][layer]")
{
	Ember::LayerStack stack;
	auto* a = new Ember::Layer("A");
	auto* b = new Ember::Layer("B");
	auto* o = new Ember::Layer("Overlay");

	stack.PushLayer(a);
	stack.PushLayer(b);
	stack.PushOverlay(o);

	REQUIRE(stack.begin() != stack.end());

	stack.PopOverlay(o);
	delete o;
	stack.PopLayer(b);
	delete b;
	stack.PopLayer(a);
	delete a;
}

TEST_CASE("ShaderCompiler GLSL to SPIR-V", "[renderer][shader]")
{
	REQUIRE(Ember::ShaderCompiler::Init());

	const char* vert = R"(
#version 450
layout(location = 0) in vec3 a_Position;
void main() {
	gl_Position = vec4(a_Position, 1.0);
}
)";

	std::vector<uint32_t> spirv;
	std::string error;
	bool ok = Ember::ShaderCompiler::CompileGLSL(vert, Ember::ShaderStage::Vertex, "main", spirv, &error);
	INFO(error);
	REQUIRE(ok);
	REQUIRE(spirv.size() > 0);

	Ember::ShaderCompiler::Shutdown();
}
