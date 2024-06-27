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
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_SceneData;

	void Renderer2D::Init()
	{
		s_SceneData = new Renderer2DStorage;

		s_SceneData->QuadVertexArray = VertexArray::Create();

		float squareVertices[4 * 5] = {
			-0.5f,  -0.5f,	0.0f, 0.0f, 0.0f,
			 0.5f,	-0.5f,	0.0f, 1.0f, 0.0f,
			 0.5f,	 0.5f,	0.0f, 1.0f, 1.0f,
			-0.5f,   0.5f,  0.0f, 0.0f, 1.0f 
		};


		Ref<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TextureCoord"}
			});


		s_SceneData->QuadVertexArray->AddVertexBuffer(squareVertexBuffer);


		uint32_t squareIndices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		Ref<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_SceneData->QuadVertexArray->SetIndexBuffer(squareIndexBuffer);

		s_SceneData->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_SceneData->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		//s_SceneData->FlatColorShader = Shader::Create("assets/shaders/FlatColorShader.glsl");
		s_SceneData->TextureShader = Shader::Create("assets/shaders/Texture.glsl");

		s_SceneData->TextureShader->SetInt("u_Texture", 0);

	}

	void Renderer2D::Shutdown()
	{
		delete s_SceneData;
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_SceneData->TextureShader->Bind();
		s_SceneData->TextureShader->SetMat4("u_VPMatrix", camera.GetVPMatrix());
		
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
		s_SceneData->TextureShader->SetFloat4("u_Color", color);
		s_SceneData->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * // Rotation
			glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		s_SceneData->TextureShader->SetMat4("u_Transform", transform);

		s_SceneData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_SceneData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		s_SceneData->TextureShader->SetFloat4("u_Color", color);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * // Rotation
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_SceneData->TextureShader->SetMat4("u_Transform", transform);


		s_SceneData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_SceneData->QuadVertexArray);
	}
}