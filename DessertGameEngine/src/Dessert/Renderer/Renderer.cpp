#include "dgepch.h"
#include "Renderer.h"

#include "glad/glad.h"

namespace Dessert {
	
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(const Camera& camera)
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		m_SceneData->m_VPMatrix = camera.GetVPMatrix() * modelMatrix;
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(
		const Ref<Shader>& shader,
		const Ref<VertexArray>& vertexArray,
		const glm::mat4& transformMatrix)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformMat4("u_MVPMatrix", m_SceneData->m_VPMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformMat4("u_ModelMatrix", transformMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}

	
