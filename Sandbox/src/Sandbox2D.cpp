#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_Texture = Dessert::Texture2D::Create("assets/textures/CheckerBoard.png");
	
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Dessert::Timestep delta)
{
	m_CameraController.OnUpdate(delta);

	Dessert::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Dessert::RenderCommand::Clear();


	Dessert::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Dessert::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Dessert::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Dessert::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 4.0f, 4.0f }, m_Texture, {0.2f, 0.3f, 0.8f, 0.5f});

	Dessert::Renderer2D::EndScene();

	//TODO Add these functions : Shader::SetMat4, Shader::SetFloat4
	//std::dynamic_pointer_cast<Dessert::OpenGLShader>(m_Shader)->Bind();
	//std::dynamic_pointer_cast<Dessert::OpenGLShader>(m_Shader)->setUniformFloat4("u_Color", m_SquareColor);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(Dessert::Event& e)
{
	m_CameraController.OnEvent(e);
}
