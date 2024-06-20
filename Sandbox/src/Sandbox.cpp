#include <Dessert.h>

#include "imgui/imgui.h"


class ExampleLayer : public Dessert::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{

		m_VertexArray.reset(Dessert::VertexArray::Create());

		float vertices[4 * 7] = {
			-0.5f,	 0.0f,	0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f,	 0.0f,	0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,	 0.5f,	0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			 0.0f,  -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<Dessert::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Dessert::VertexBuffer::Create(vertices, sizeof(vertices)));


		Dessert::BufferLayout layout = {
			{Dessert::ShaderDataType::Float3, "a_Position"},
			{Dessert::ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = {
			0, 1, 2,
			0, 1, 3
		};

		std::shared_ptr<Dessert::IndexBuffer> indexBuffer;
		indexBuffer.reset(Dessert::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVertexArray.reset(Dessert::VertexArray::Create());

		float squareVertices[4 * 3] = {
			-0.5f,	-0.5f,	0.0f,
			 0.5f,	-0.5f,	0.0f,
			 0.5f,	 0.5f,	0.0f,
			-0.5f,   0.5f,  0.0f
		};


		std::shared_ptr<Dessert::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Dessert::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVertexBuffer->SetLayout({
			{Dessert::ShaderDataType::Float3, "a_Position"}
			});

		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);


		uint32_t squareIndices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		std::shared_ptr<Dessert::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Dessert::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

		m_Shader.reset(new Dessert::Shader(vertexSrc, fragmentSrc));

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

		m_BlueShader.reset(new Dessert::Shader(blueVertexSrc, blueFragmentSrc));


	}

	void OnUpdate() override
	{
		//DGE_INFO("ExampleLayer::Update");

		if (Dessert::Input::isKeyPressed(DGE_KEY_TAB))
			DGE_TRACE("TAB key is pressed");

		Dessert::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		Dessert::RenderCommand::Clear();

		Dessert::Renderer::BeginScene();

		m_BlueShader->Bind();
		Dessert::Renderer::Submit(m_SquareVertexArray);

		m_Shader->Bind();
		Dessert::Renderer::Submit(m_VertexArray);

		Dessert::Renderer::EndScene();
	}

	void OnEvent(Dessert::Event& event) override
	{
		if (event.GetEventType() == Dessert::EventType::KeyPressed)
		{
			Dessert::KeyPressedEvent& e = (Dessert::KeyPressedEvent&)event;
			DGE_TRACE("{0}", (char)e.getKeyCode());
		}
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello world");
		ImGui::ColorEdit4("", new float[4]);
		ImGui::End();
	}

	private:
		std::shared_ptr<Dessert::Shader> m_Shader;
		std::shared_ptr<Dessert::VertexArray> m_VertexArray;

		std::shared_ptr<Dessert::Shader> m_BlueShader;
		std::shared_ptr<Dessert::VertexArray> m_SquareVertexArray;
};

class Sandbox : public Dessert::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Dessert::Application* Dessert::CreateApplication()
{
	return new Sandbox();
}
