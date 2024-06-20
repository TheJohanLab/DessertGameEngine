#include "dgepch.h"

#include "Application.h"

#include <glad/glad.h>

#include "Input.h"


namespace Dessert {

	Application* Application::s_Instance = nullptr;




	Application::Application()
	{
		DGE_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNC(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();

		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());

		float vertices[4 * 7] = {
			-0.5f,	 0.0f,	0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f,	 0.0f,	0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,	 0.5f,	0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			 0.0f,  -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = {
			0, 1, 2,
			0, 1, 3
		};

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVertexArray.reset(VertexArray::Create());

		float squareVertices[4 * 3] = {
			-0.5f,	-0.5f,	0.0f, 
			 0.5f,	-0.5f,	0.0f, 
			 0.5f,	 0.5f,	0.0f, 
			-0.5f,   0.5f,  0.0f 
		};


		std::shared_ptr<VertexBuffer> squareVertexBuffer; 
		squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"}
			});

		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);


		uint32_t squareIndices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		std::shared_ptr<IndexBuffer> squareIndexBuffer; 
		squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		//Shaders
		std::string vertexSrc = R"(

			#version 330 core

			layout(location = 0) in vec3 a_Position;			
			layout(location = 1) in vec4 a_Color;			
			
			out vec3 v_Position;
			out vec4 v_Color;


			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(

			#version 330 core

			layout(location = 0) out vec4 color;			
			
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}

		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		//Shaders
		std::string blueVertexSrc = R"(

			#version 330 core

			layout(location = 0) in vec3 a_Position;			
			
			out vec3 v_Position;


			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string blueFragmentSrc = R"(

			#version 330 core

			layout(location = 0) out vec4 color;			
			
			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}

		)";

		m_BlueShader.reset(new Shader(blueVertexSrc, blueFragmentSrc));

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{

		while (m_Running) 
		{
			glClearColor(0.2f, 0.2f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_BlueShader->Bind();
			m_SquareVertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
			
			m_Shader->Bind();
			m_VertexArray->Bind();

			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
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

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(Application::OnWindowClose));

		DGE_CORE_TRACE("{0}", event);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
						
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}


}
