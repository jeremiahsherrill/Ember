#include "empch.h"
#include "Ember/Core/Application.h"
#include "Ember/Core/Log.h"
#include "Ember/Core/Input.h"
#include "Ember/Renderer/Renderer.h"
#include "Ember/ImGui/ImGuiLayer.h"

#include <nvrhi/utils.h>
#include <GLFW/glfw3.h>

namespace Ember {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		EM_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		WindowProps props;
		props.Title = m_Specification.Name;
		props.Width = m_Specification.Width;
		props.Height = m_Specification.Height;
		props.Headless = m_Specification.Headless;

		m_Window = Window::Create(props);
		m_Window->SetEventCallback(EM_BIND_EVENT_FN(Application::OnEvent));

		m_GraphicsDevice = CreateScope<GraphicsDevice>();
		bool ok = m_GraphicsDevice->Init(m_Window->GetGLFWWindow(), m_Specification.EnableValidation);
		EM_CORE_ASSERT(ok, "Failed to initialize GraphicsDevice!");

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		Renderer::Shutdown();
		m_GraphicsDevice->Shutdown();
		s_Instance = nullptr;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::SubmitToMainThread(const std::function<void()>& function)
	{
		std::scoped_lock lock(m_MainThreadQueueMutex);
		m_MainThreadQueue.emplace_back(function);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(EM_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(EM_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			ExecuteMainThreadQueue();

			if (!m_Minimized)
			{
				m_GraphicsDevice->BeginFrame();

				{
					nvrhi::ICommandList* cmd = m_GraphicsDevice->GetCommandList();
					nvrhi::IFramebuffer* fb = m_GraphicsDevice->GetCurrentFramebuffer();

					nvrhi::utils::ClearColorAttachment(cmd, fb, 0, nvrhi::Color(0.08f, 0.08f, 0.10f, 1.0f));

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);

					m_ImGuiLayer->Begin();
					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
					m_ImGuiLayer->End();
				}

				m_GraphicsDevice->EndFrame();
				m_GraphicsDevice->Present();
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		m_GraphicsDevice->ResizeSwapchain(e.GetWidth(), e.GetHeight());
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}

	void Application::ExecuteMainThreadQueue()
	{
		std::scoped_lock lock(m_MainThreadQueueMutex);
		for (auto& func : m_MainThreadQueue)
			func();
		m_MainThreadQueue.clear();
	}

}
