#include <Ember.h>
#include <Ember/Core/EntryPoint.h>

namespace Ember {

	class RuntimeLayer : public Layer
	{
	public:
		RuntimeLayer() : Layer("RuntimeLayer") {}

		void OnAttach() override
		{
			EM_INFO("Ember Runtime started");
		}

		void OnImGuiRender() override
		{
			// Runtime has no editor UI; ImGui overlay may still exist for debug
		}
	};

	class EmberRuntime : public Application
	{
	public:
		EmberRuntime(const ApplicationSpecification& spec)
			: Application(spec)
		{
			PushLayer(new RuntimeLayer());
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Ember Runtime";
		spec.CommandLineArgs = args;
		spec.Width = 1280;
		spec.Height = 720;

		return new EmberRuntime(spec);
	}

}
