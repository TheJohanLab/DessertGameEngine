#include "dgepch.h"
#include "Renderer.h"

#include "glad/glad.h"

namespace Dessert {
	
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(const Camera& camera)
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		m_SceneData->m_VPMatrix = camera.GetVPMatrix() * modelMatrix;
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(
		const std::shared_ptr<Shader>& shader, 
		const std::shared_ptr<VertexArray>& vertexArray,
		const glm::mat4& transformMatrix)
	{
		shader->Bind();
		shader->setUniformMat4f("u_MVPMatrix", m_SceneData->m_VPMatrix);
		shader->setUniformMat4f("u_ModelMatrix", transformMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}

	
