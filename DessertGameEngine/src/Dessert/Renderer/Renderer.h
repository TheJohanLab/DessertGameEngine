#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

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

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetRendererAPI() { return RendererAPI::GetAPI(); }
	
	};
}