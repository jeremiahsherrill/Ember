#pragma once

#include "Ember/Core/Base.h"
#include "Ember/Core/Application.h"
#include "Ember/Core/Log.h"

#ifdef EM_PLATFORM_WINDOWS

extern Ember::Application* Ember::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	Ember::Log::Init();
	EM_CORE_INFO("Initialized Ember Logger");

	auto app = Ember::CreateApplication({ argc, argv });
	app->Run();
	delete app;
}

#elif defined(EM_PLATFORM_LINUX) || defined(EM_PLATFORM_MACOS)

extern Ember::Application* Ember::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	Ember::Log::Init();
	EM_CORE_INFO("Initialized Ember Logger");

	auto app = Ember::CreateApplication({ argc, argv });
	app->Run();
	delete app;
}

#endif
