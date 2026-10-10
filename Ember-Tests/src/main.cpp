#include <catch2/catch_session.hpp>

#include <Ember/Core/Log.h>

int main(int argc, char* argv[])
{
	Ember::Log::Init();
	return Catch::Session().run(argc, argv);
}
