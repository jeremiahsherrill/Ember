#include <Ember.h>
#include <Ember/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Ember {

	class EmberEditor : public Application
	{
	public:
		EmberEditor(const ApplicationSpecification& spec)
			: Application(spec)
		{
			PushLayer(new EditorLayer());
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Ember Editor";
		spec.CommandLineArgs = args;
		spec.Width = 1600;
		spec.Height = 900;

		for (int i = 0; i < args.Count; i++)
		{
			std::string arg = args[i] ? args[i] : "";
			if (arg == "--headless")
				spec.Headless = true;
			if (arg == "--no-validation")
				spec.EnableValidation = false;
		}

		return new EmberEditor(spec);
	}

}
