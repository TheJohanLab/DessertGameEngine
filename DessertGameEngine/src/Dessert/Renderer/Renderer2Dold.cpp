#include "dgepch.h"
#include "Renderer2Dold.h"

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

	void Renderer2Dold::Init()
	{
		DGE_PROFILE_FUNCTION();

		s_SceneData = new Renderer2DStorage;

		s_SceneData->QuadVertexArray = VertexArray::Create();

		float squareVertices[4 * 5] = {
			-0.5f,  -0.5f,	0.0f, 0.0f, 0.0f,
			 0.5f,	-0.5f,	0.0f, 1.0f, 0.0f,
			 0.5f,	 0.5f,	0.0f, 1.0f, 1.0f,
			-0.5f,   0.5f,  0.0f, 0.0f, 1.0f 
		};


		Ref<VertexBuffer> squareVertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		squareVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TextureCoord"}
			});


		s_SceneData->QuadVertexArray->AddVertexBuffer(squareVertexBuffer);


		uint32_t squareIndices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		Ref<IndexBuffer> squareIndexBuffer = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_SceneData->QuadVertexArray->SetIndexBuffer(squareIndexBuffer);

		s_SceneData->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_SceneData->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		//s_SceneData->FlatColorShader = Shader::Create("assets/shaders/FlatColorShader.glsl");
		s_SceneData->TextureShader = Shader::Create("assets/shaders/TextureOld.glsl");

		s_SceneData->TextureShader->SetInt("u_Texture", 0);

	}

	void Renderer2Dold::Shutdown()
	{
		DGE_PROFILE_FUNCTION();

		delete s_SceneData;
	}

	void Renderer2Dold::BeginScene(const Camera& camera)
	{
		DGE_PROFILE_FUNCTION();

		s_SceneData->TextureShader->Bind();
		s_SceneData->TextureShader->SetMat4("u_VPMatrix", camera.GetVPMatrix());
		
	}

	void Renderer2Dold::EndScene()
	{
		DGE_PROFILE_FUNCTION();

	}

	void Renderer2Dold::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);

	}

	void Renderer2Dold::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DGE_PROFILE_FUNCTION();

		s_SceneData->TextureShader->SetFloat4("u_Color", color);
		s_SceneData->TextureShader->SetFloat("u_TileMultiplier", 1.0f);
		s_SceneData->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * // Rotation
			glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		s_SceneData->TextureShader->SetMat4("u_Transform", transform);

		s_SceneData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_SceneData->QuadVertexArray);
	}

	void Renderer2Dold::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tileMultiplier, color);
	}

	void Renderer2Dold::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& color)
	{
		DGE_PROFILE_FUNCTION();

		s_SceneData->TextureShader->SetFloat4("u_Color", color);
		s_SceneData->TextureShader->SetFloat("u_TileMultiplier", tileMultiplier);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * // Rotation
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_SceneData->TextureShader->SetMat4("u_Transform", transform);


		s_SceneData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_SceneData->QuadVertexArray);
	}

	void Renderer2Dold::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2Dold::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DGE_PROFILE_FUNCTION();

		s_SceneData->TextureShader->SetFloat4("u_Color", color);
		s_SceneData->TextureShader->SetFloat("u_TileMultiplier", 1.0f);
		s_SceneData->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0), position) 
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_SceneData->TextureShader->SetMat4("u_Transform", transform);

		s_SceneData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_SceneData->QuadVertexArray);
	}

	void Renderer2Dold::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tileMultiplier, color);
	}

	void Renderer2Dold::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& color)
	{
		DGE_PROFILE_FUNCTION();

		s_SceneData->TextureShader->SetFloat4("u_Color", color);
		s_SceneData->TextureShader->SetFloat("u_TileMultiplier", tileMultiplier);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_SceneData->TextureShader->SetMat4("u_Transform", transform);


		s_SceneData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_SceneData->QuadVertexArray);
	}
}