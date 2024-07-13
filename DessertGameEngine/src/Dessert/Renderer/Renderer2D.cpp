#include "dgepch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Dessert {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};

	struct Renderer2DData
	{
		const uint32_t maxQuads = 10000;
		const uint32_t maxVertices = maxQuads * 4;
		const uint32_t maxIndices = maxQuads * 6;


		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t quadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr  = nullptr;
	};

	static Renderer2DData s_SceneData;

	

	void Renderer2D::Init()
	{
		DGE_PROFILE_FUNCTION();

		s_SceneData.QuadVertexArray = VertexArray::Create();


		s_SceneData.QuadVertexBuffer = VertexBuffer::Create(s_SceneData.maxIndices * sizeof(QuadVertex));

		s_SceneData.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TextureCoord"}
			});


		s_SceneData.QuadVertexArray->AddVertexBuffer(s_SceneData.QuadVertexBuffer);

		s_SceneData.QuadVertexBufferBase = new QuadVertex[s_SceneData.maxVertices];

		uint32_t* quadIndices = new uint32_t[s_SceneData.maxIndices];

		uint32_t offset = 0;

		for (uint32_t i = 0; i < s_SceneData.maxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
										   
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> squareIndexBuffer = IndexBuffer::Create(quadIndices, s_SceneData.maxIndices);
		s_SceneData.QuadVertexArray->SetIndexBuffer(squareIndexBuffer);
		
		delete[] quadIndices;


		s_SceneData.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_SceneData.WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		//s_SceneData.FlatColorShader = Shader::Create("assets/shaders/FlatColorShader.glsl");
		s_SceneData.TextureShader = Shader::Create("assets/shaders/Texture.glsl");

		s_SceneData.TextureShader->SetInt("u_Texture", 0);

	}

	void Renderer2D::Shutdown()
	{
		DGE_PROFILE_FUNCTION();

	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		DGE_PROFILE_FUNCTION();

		s_SceneData.TextureShader->Bind();
		s_SceneData.TextureShader->SetMat4("u_VPMatrix", camera.GetVPMatrix());

		s_SceneData.quadIndexCount = 0;
		s_SceneData.QuadVertexBufferPtr = s_SceneData.QuadVertexBufferBase;

	}

	void Renderer2D::EndScene()
	{
		DGE_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_SceneData.QuadVertexBufferPtr - (uint8_t*)s_SceneData.QuadVertexBufferBase;
		s_SceneData.QuadVertexBuffer->SetData(s_SceneData.QuadVertexBufferBase, dataSize);

		Flush();

	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(s_SceneData.QuadVertexArray, s_SceneData.quadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);

	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DGE_PROFILE_FUNCTION();

		s_SceneData.QuadVertexBufferPtr->Position = position;
		s_SceneData.QuadVertexBufferPtr->Color = color;
		s_SceneData.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_SceneData.QuadVertexBufferPtr++;

		s_SceneData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_SceneData.QuadVertexBufferPtr->Color = color;
		s_SceneData.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_SceneData.QuadVertexBufferPtr++;

		s_SceneData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_SceneData.QuadVertexBufferPtr->Color = color;
		s_SceneData.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_SceneData.QuadVertexBufferPtr++;

		s_SceneData.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_SceneData.QuadVertexBufferPtr->Color = color;
		s_SceneData.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_SceneData.QuadVertexBufferPtr++;

		s_SceneData.quadIndexCount += 6;

		//s_SceneData.TextureShader->SetFloat("u_TileMultiplier", 1.0f);
		//s_SceneData.WhiteTexture->Bind();

		//glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * // Rotation
		//	glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		//s_SceneData.TextureShader->SetMat4("u_Transform", transform);

		//s_SceneData.QuadVertexArray->Bind();
		//RenderCommand::DrawIndexed(s_SceneData.QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tileMultiplier, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& color)
	{
		DGE_PROFILE_FUNCTION();

		s_SceneData.TextureShader->SetFloat4("u_Color", color);
		s_SceneData.TextureShader->SetFloat("u_TileMultiplier", tileMultiplier);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * // Rotation
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_SceneData.TextureShader->SetMat4("u_Transform", transform);


		s_SceneData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_SceneData.QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DGE_PROFILE_FUNCTION();

		s_SceneData.TextureShader->SetFloat4("u_Color", color);
		s_SceneData.TextureShader->SetFloat("u_TileMultiplier", 1.0f);
		s_SceneData.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_SceneData.TextureShader->SetMat4("u_Transform", transform);

		s_SceneData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_SceneData.QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tileMultiplier, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& color)
	{
		DGE_PROFILE_FUNCTION();

		s_SceneData.TextureShader->SetFloat4("u_Color", color);
		s_SceneData.TextureShader->SetFloat("u_TileMultiplier", tileMultiplier);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_SceneData.TextureShader->SetMat4("u_Transform", transform);


		s_SceneData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_SceneData.QuadVertexArray);
	}
}