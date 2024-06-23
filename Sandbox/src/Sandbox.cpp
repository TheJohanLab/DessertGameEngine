#include <Dessert.h>
#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Dessert::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), 
		m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
		m_CameraPosition(0.0f, 0.0f, 0.0f),
		m_CameraRotation(0.0f, 0.0f, 0.0f),
		m_CameraScale(1.0f, 1.0f, 1.0f)
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

		m_Shader.reset(Dessert::Shader::Create(vertexSrc, fragmentSrc));

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

		m_flatColorShader.reset(Dessert::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));
		
		/*Dessert::Material* material = new Dessert::Material(m_flatColorShader);
		Dessert::MaterialInstanceRef* materialInstance = new Dessert::MaterialInstance(material);*/

		//materialInstance->Set("u_Color", redColor);
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

		if (Dessert::Input::isKeyPressed(DGE_KEY_P))
		{
			m_CameraScale.x -= 0.11 * delta;
			m_CameraScale.y -= 0.11 * delta;
		}
		else if (Dessert::Input::isKeyPressed(DGE_KEY_Z))
		{
			m_CameraScale.x += 0.11 * delta;
			m_CameraScale.y += 0.11 * delta;
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




		Dessert::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Dessert::RenderCommand::Clear();

		m_Camera.setTransformPosition(m_CameraPosition);
		m_Camera.setTransformRotation(m_CameraRotation);
		m_Camera.setTransformScale(m_CameraScale);

		Dessert::Renderer::BeginScene(m_Camera);
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
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}

	private:
		std::shared_ptr<Dessert::Shader> m_Shader;
		std::shared_ptr<Dessert::VertexArray> m_VertexArray;

		std::shared_ptr<Dessert::Shader> m_flatColorShader;
		std::shared_ptr<Dessert::VertexArray> m_SquareVertexArray;

		Dessert::Camera m_Camera;

		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraRotation;
		glm::vec3 m_CameraScale;


		float m_CameraMoveSpeed = 1.0f;
		float m_CameraRotationSpeed = 50.0f;

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
