#include <Dessert.h>

#include "imgui/imgui.h"


class ExampleLayer : public Dessert::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), 
		m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
		m_CameraPosition(0.0f, 0.0f, 0.0f),
		m_CameraRotation(0.0f, 0.0f, 0.0f)
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
			-0.5f,  -0.5f,	0.0f,
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

		m_Shader.reset(new Dessert::Shader(vertexSrc, fragmentSrc));

		//Shaders
		std::string blueVertexSrc = R"(

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

	void OnUpdate(Dessert::Timestep delta) override
	{
		//DGE_TRACE("Delta time: {0} ({1}ms)", delta.GetSeconds(), delta.GetMilliseconds());

		// --------- Camera ------------

		if (Dessert::Input::isKeyPressed(DGE_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * delta;
		}
		else if (Dessert::Input::isKeyPressed(DGE_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * delta;
		}

		if (Dessert::Input::isKeyPressed(DGE_KEY_UP))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * delta;
		}
		else if (Dessert::Input::isKeyPressed(DGE_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * delta;
		}

		if (Dessert::Input::isKeyPressed(DGE_KEY_A))
		{
			m_CameraRotation.z += m_CameraRotationSpeed * delta;
		}
		if (Dessert::Input::isKeyPressed(DGE_KEY_D))
		{
			m_CameraRotation.z -= m_CameraRotationSpeed * delta;
		}




		Dessert::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		Dessert::RenderCommand::Clear();

		m_Camera.setTransformPosition(m_CameraPosition);
		m_Camera.setTransformRotation(m_CameraRotation);

		Dessert::Renderer::BeginScene(m_Camera);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 10; x++)
			{
				glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
				Dessert::Renderer::Submit(m_BlueShader, m_SquareVertexArray, transform);	
			}
		}

		Dessert::Renderer::Submit(m_Shader, m_VertexArray);

		Dessert::Renderer::EndScene();
	}

	void OnEvent(Dessert::Event& event) override
	{
		//Dessert::EventDispatcher dispatcher(event);
		//dispatcher.Dispatch<Dessert::KeyPressedEvent>(BIND_EVENT_FUNC(ExampleLayer::OnKeyPressed));

	}


	virtual void OnImGuiRender() override
	{
	}

	private:
		std::shared_ptr<Dessert::Shader> m_Shader;
		std::shared_ptr<Dessert::VertexArray> m_VertexArray;

		std::shared_ptr<Dessert::Shader> m_BlueShader;
		std::shared_ptr<Dessert::VertexArray> m_SquareVertexArray;

		Dessert::Camera m_Camera;

		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraRotation;

		float m_CameraMoveSpeed = 1.0f;
		float m_CameraRotationSpeed = 50.0f;

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
