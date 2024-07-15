#pragma once

#include "Dessert.h"

#include "ParticleSystemTmp.h"

class Sandbox2D: public Dessert::Layer
{

private:
	Dessert::OrthographicCameraController m_CameraController;

	Dessert::Ref<Dessert::VertexArray> m_VertexArray;
	Dessert::ShaderLibrary m_ShaderLibrary;
	Dessert::Ref<Dessert::Shader> m_Shader;

	Dessert::Ref<Dessert::Texture2D> m_Texture;

	glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 0.1f };

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	float m_LastDeltaTime = 0.0f;

	ParticleSystemTmp m_ParticleSystem = (10000);
	ParticlePropsTmp m_Particles;
	//std::vector<ProfileResult> m_ProfileResults;

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