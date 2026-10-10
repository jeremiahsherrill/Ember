#pragma once

#include "Ember/Core/Base.h"
#include "Ember/Core/Window.h"
#include "Ember/Core/LayerStack.h"
#include "Ember/Core/Timestep.h"
#include "Ember/Events/Event.h"
#include "Ember/Events/ApplicationEvent.h"
#include "Ember/Renderer/GraphicsDevice.h"

namespace Ember {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			EM_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "Ember Application";
		std::string WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
		uint32_t Width = 1600;
		uint32_t Height = 900;
		bool Headless = false;
		bool EnableValidation = true;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return *m_Window; }
		GraphicsDevice& GetGraphicsDevice() { return *m_GraphicsDevice; }

		class ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		void SubmitToMainThread(const std::function<void()>& function);

		static Application& Get() { return *s_Instance; }
		const ApplicationSpecification& GetSpecification() const { return m_Specification; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		void ExecuteMainThreadQueue();

	private:
		ApplicationSpecification m_Specification;
		Scope<Window> m_Window;
		Scope<GraphicsDevice> m_GraphicsDevice;
		class ImGuiLayer* m_ImGuiLayer = nullptr;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		std::vector<std::function<void()>> m_MainThreadQueue;
		std::mutex m_MainThreadQueueMutex;

		static Application* s_Instance;
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);

}
