#pragma once

#include "Dessert.h"

class Sandbox2D: public Dessert::Layer
{

private:
	Dessert::OrthographicCameraController m_CameraController;

	Dessert::Ref<Dessert::VertexArray> m_VertexArray;
	Dessert::ShaderLibrary m_ShaderLibrary;
	Dessert::Ref<Dessert::Shader> m_Shader;

	glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 0.1f };

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Dessert::Timestep delta) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Dessert::Event& event) override;

private:

};