#pragma once

#include "Core.h"

#include "Window.h"
#include "Dessert/LayerStack.h"
#include "Dessert/Events/Event.h"

#include "Dessert/Events/ApplicationEvent.h"

#include "Dessert/ImGui/ImGuiLayer.h"


namespace Dessert {

	class DESSERT_API Application
	{
	public: 
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
	
		inline static Application& Get() { return *s_Instance; }


	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
	
	private:
		static Application* s_Instance;
	};

	//To be defined in the client
	Application* CreateApplication();

}

