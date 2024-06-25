#include <Dessert.h>
#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Dessert::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), 
	    m_CameraController(1280.0f / 720.0f, true)
	{

		m_VertexArray.reset(Dessert::VertexArray::Create());

		float vertices[4 * 7] = {
			-0.5f,	 0.0f,	0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f,	 0.0f,	0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,	 0.5f,	0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			 0.0f,  -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};


		Dessert::Ref<Dessert::VertexBuffer> vertexBuffer;
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

		Dessert::Ref<Dessert::IndexBuffer> indexBuffer;
		indexBuffer.reset(Dessert::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVertexArray.reset(Dessert::VertexArray::Create());

		float squareVertices[4 * 5] = {
			-0.5f,  -0.5f,	0.0f, 0.0f, 0.0f,
			 0.5f,	-0.5f,	0.0f, 1.0f, 0.0f,
			 0.5f,	 0.5f,	0.0f, 1.0f, 1.0f,
			-0.5f,   0.5f,  0.0f, 0.0f, 1.0f
		};


		Dessert::Ref<Dessert::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Dessert::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVertexBuffer->SetLayout({
			{Dessert::ShaderDataType::Float3, "a_Position"},
			{Dessert::ShaderDataType::Float2, "a_TextureCoord"}
		});


		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);


		uint32_t squareIndices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		Dessert::Ref<Dessert::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Dessert::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		//Shaders
		std::string vertexSrc = R"(

			#version 330 core

			layout(location = 0) in vec3 a_Position;			
			layout(location = 1) in vec4 a_Color;			
			
			out vec3 v_Position;
			out vec4 v_Color;
			uniform mat4 u_MVPMatrix;
			uniform mat4 u_ModelMatrix;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_MVPMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
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

		m_Shader = Dessert::Shader::Create("VertexColorLosange", vertexSrc, fragmentSrc);

		//Shaders
		std::string flatColorShaderVertexSrc = R"(

			#version 330 core

			layout(location = 0) in vec3 a_Position;			
			
			out vec3 v_Position;
			uniform mat4 u_MVPMatrix;
			uniform mat4 u_ModelMatrix;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_MVPMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
			}

		)";

		std::string flatColorShaderFragmentSrc = R"(

			#version 330 core

			layout(location = 0) out vec4 color;			
			
			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}

		)";

		m_flatColorShader = Dessert::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);


		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		
		m_Texture = Dessert::Texture2D::Create("assets/textures/CheckerBoard.png");
		m_ButtonTexture = Dessert::Texture2D::Create("assets/textures/pushBtn.png");

		std::dynamic_pointer_cast<Dessert::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Dessert::OpenGLShader>(textureShader)->setUniformInt("u_Texture", 0); // 0 is the texture slot
		/*Dessert::Material* material = new Dessert::Material(m_flatColorShader);
		Dessert::MaterialInstanceRef* materialInstance = new Dessert::MaterialInstance(material);*/

		//materialInstance->Set("u_Color", redColor);
	}

	void OnUpdate(Dessert::Timestep delta) override
	{
		m_CameraController.OnUpdate(delta);

		Dessert::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Dessert::RenderCommand::Clear();


		Dessert::Renderer::BeginScene(m_CameraController.GetCamera());
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Dessert::OpenGLShader>(m_flatColorShader)->Bind();
		std::dynamic_pointer_cast<Dessert::OpenGLShader>(m_flatColorShader)->setUniformFloat3("u_Color", m_SquareColor);


		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 10; x++)
			{
				glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;

				Dessert::Renderer::Submit(m_flatColorShader, m_SquareVertexArray, transform);
				//Dessert::Renderer::Submit(materialInstance, m_SquareVertexArray, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");
		m_Texture->Bind();
		Dessert::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
		m_ButtonTexture->Bind();
		Dessert::Renderer::Submit(textureShader, m_SquareVertexArray,
			 glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
		//Dessert::Renderer::Submit(m_Shader, m_VertexArray);

		Dessert::Renderer::EndScene();
	}

	void OnEvent(Dessert::Event& e) override
	{
		m_CameraController.OnEvent(e);

		if (e.GetEventType() == Dessert::EventType::WindowResize)
		{
			auto& resizeEvent = (Dessert::WindowResizeEvent&)e;

		}

		//Dessert::EventDispatcher dispatcher(event);
		//dispatcher.Dispatch<Dessert::KeyPressedEvent>(BIND_EVENT_FUNC(ExampleLayer::OnKeyPressed));

	}


	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}

	private:
		Dessert::ShaderLibrary m_ShaderLibrary;

		Dessert::Ref<Dessert::Shader> m_Shader;
		Dessert::Ref<Dessert::VertexArray> m_VertexArray;

		Dessert::Ref<Dessert::Shader> m_flatColorShader;
		Dessert::Ref<Dessert::VertexArray> m_SquareVertexArray;

		Dessert::Ref<Dessert::Texture2D> m_Texture;
		Dessert::Ref<Dessert::Texture2D> m_ButtonTexture;

		Dessert::OrthographicCameraController m_CameraController;

		glm::vec3 m_SquareColor = { 0.8f, 0.2f, 0.3f };

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
