#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include "Dessert/Debug/Profiler.h"

//template<typename Fn>
//class Timer
//{
//public:
//	Timer(const char* name, Fn&& func)
//		:m_Name(name), m_Func(func), m_Stopped(false)
//	{
//		m_StartTimePoint = std::chrono::high_resolution_clock::now();
//	}
//
//	~Timer()
//	{
//		if (!m_Stopped)
//			Stop();
//	}
//
//	void Stop()
//	{
//		auto endTimePoint = std::chrono::high_resolution_clock::now();
//
//		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
//		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
//
//		m_Stopped = true;
//	
//		float duration = (end - start) * 0.001f;
//		m_Func({ m_Name, duration });
//	}
//
//	
//private:
//	const char* m_Name;
//	Fn m_Func;
//	std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
//	bool m_Stopped;
//};
//
//#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

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
	//PROFILE_SCOPE("Sandbox2D::OnUpdate"); Ancienne version
	DGE_PROFILE_SCOPE("name")
		
	{
		DGE_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(delta);
	}

	{
		DGE_PROFILE_SCOPE("Renderer Prep");
		Dessert::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Dessert::RenderCommand::Clear();
	}

	{
		DGE_PROFILE_SCOPE("Renderer Draw");
		Dessert::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Dessert::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Dessert::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Dessert::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 4.0f, 4.0f }, m_Texture, { 0.2f, 0.3f, 0.8f, 0.5f });

		Dessert::Renderer2D::EndScene();
	}
	//TODO Add these functions : Shader::SetMat4, Shader::SetFloat4
	//std::dynamic_pointer_cast<Dessert::OpenGLShader>(m_Shader)->Bind();
	//std::dynamic_pointer_cast<Dessert::OpenGLShader>(m_Shader)->setUniformFloat4("u_Color", m_SquareColor);
}

void Sandbox2D::OnImGuiRender()
{
	DGE_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	/*for (auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, " %.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}

	m_ProfileResults.clear();*/
	ImGui::End();
}

void Sandbox2D::OnEvent(Dessert::Event& e)
{
	m_CameraController.OnEvent(e);
}
