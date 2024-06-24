#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Dessert{

	class Renderer
	{
	private:
		struct SceneData
		{
			glm::mat4 m_VPMatrix;
		};

		static SceneData* m_SceneData;

	public:
		static void BeginScene(const Camera& camera);
		static void EndScene();

		static void Submit(
			const Ref<Shader>& shader,
			const Ref<VertexArray>& vertexArray,
			const glm::mat4& transformMatrix = glm::mat4(1.0f));

		inline static RendererAPI::API GetRendererAPI() { return RendererAPI::GetAPI(); }
	
	};
}