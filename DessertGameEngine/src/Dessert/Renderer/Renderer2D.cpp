#include "dgepch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Dessert {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_SceneData;

	void Renderer2D::Init()
	{
		s_SceneData = new Renderer2DStorage;

		s_SceneData->QuadVertexArray = VertexArray::Create();

		float squareVertices[4 * 3] = {
			-0.5f,  -0.5f,	0.0f,
			 0.5f,	-0.5f,	0.0f,
			 0.5f,	 0.5f,	0.0f,
			-0.5f,   0.5f,  0.0f
		};


		Ref<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"}
			});


		s_SceneData->QuadVertexArray->AddVertexBuffer(squareVertexBuffer);


		uint32_t squareIndices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		Ref<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_SceneData->QuadVertexArray->SetIndexBuffer(squareIndexBuffer);

		s_SceneData->FlatColorShader = Shader::Create("assets/shaders/FlatColorShader.glsl");

	}

	void Renderer2D::Shutdown()
	{
		delete s_SceneData;
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_SceneData->FlatColorShader->Bind();
		s_SceneData->FlatColorShader->SetMat4("u_VPMatrix", camera.GetVPMatrix());
		
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);

	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_SceneData->FlatColorShader->Bind();
		s_SceneData->FlatColorShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * // Rotation
			glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		s_SceneData->FlatColorShader->SetMat4("u_Transform", transform);

		s_SceneData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_SceneData->QuadVertexArray);
	}
}