#include "dgepch.h"

#include "Application.h"

#include "Dessert/Renderer/Renderer.h"

#include "Input.h"

#include "GLFW/glfw3.h"


namespace Dessert {

	Application* Application::s_Instance = nullptr;


	Application::Application()
		
	{
		initApplication();
		
	}

	Application::~Application()
	{
		DGE_PROFILE_FUNCTION();
	}

	void Application::Run()
	{
		DGE_PROFILE_FUNCTION();

		/*const double targetFPS = 60.0;
		const double targetFrameTime = 1.0 / targetFPS;*/

		while (m_Running) 
		{
			DGE_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					DGE_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();
				{
					DGE_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();

			/*
			double frameDuration = glfwGetTime() - time;
			while (frameDuration < targetFrameTime)
			{
				frameDuration = glfwGetTime() - time;
			}
			*/
		}
	}


	void Application::PushLayer(Layer* layer)
	{
		DGE_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		DGE_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& event)
	{
		DGE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(Application::OnWindowResize));

		DGE_CORE_TRACE("{0}", event);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
						
		}
	}

	void Application::initApplication()
	{
		DGE_PROFILE_FUNCTION();

		DGE_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNC(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();

		PushOverlay(m_ImGuiLayer);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		DGE_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}


}
