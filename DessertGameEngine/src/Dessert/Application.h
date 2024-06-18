#pragma once

#include "Core.h"

#include "Window.h"
#include "Dessert/LayerStack.h"
#include "Dessert/Events/Event.h"

#include "Dessert/Events/ApplicationEvent.h"

#include "Dessert/ImGui/ImGuiLayer.h"

#include "Dessert/Renderer/Shader.h"
#include "Dessert/Renderer/Buffer.h"

namespace Dessert {

	class DESSERT_API Application
	{
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		static Application* s_Instance;

		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

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
	};

	//To be defined in the client
	Application* CreateApplication();

}

