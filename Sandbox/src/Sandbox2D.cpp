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
//		:m_Name(name), m_Func(std::forward<Fn>(func)), m_Stopped(false)
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
	DGE_PROFILE_FUNCTION()

	m_Texture = Dessert::Texture2D::Create("assets/textures/CheckerBoard.png");
	

	m_Particles.Position = { 0.0f, 0.0f };
	m_Particles.Velocity = { -2.0f, 0.0f }, m_Particles.VelocityVariation = { 3.0f, 1.0f };
	m_Particles.SizeBegin = 0.5f, m_Particles.SizeEnd = 0.0f, m_Particles.SizeVariation = 0.3f;
	m_Particles.ColorBegin = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particles.ColorEnd = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f , 1.0f };
	m_Particles.LifeTime = 5.0f;

}

void Sandbox2D::OnDetach()
{
	DGE_PROFILE_FUNCTION()
}

void Sandbox2D::OnUpdate(Dessert::Timestep delta)
{
	//PROFILE_SCOPE("Sandbox2D::OnUpdate"); Ancienne version
	DGE_PROFILE_FUNCTION()

	m_LastDeltaTime = delta;

	m_CameraController.OnUpdate(delta);
	
	Dessert::Renderer2D::ResetStats();

	{
		DGE_PROFILE_SCOPE("Renderer Prep");
		Dessert::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Dessert::RenderCommand::Clear();
	}

	{
		//static float rotation = 0.0f;
		//rotation += delta * 20.0f;
		DGE_PROFILE_SCOPE("Renderer Draw");

		Dessert::Renderer2D::BeginScene(m_CameraController.GetCamera());

		//Dessert::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(rotation), { 0.5f, 0.2f, 0.3f, 1.0f });
		//Dessert::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		//Dessert::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Dessert::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_Texture, 10.0f);
		//Dessert::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_Texture, 20.0f);


		//for (float y = -5.0f; y < 5.0f; y += 0.5f)
		//{
		//	for (float x = -5.0f; x < 5.0f; x += 0.5f)
		//	{
		//		glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
		//		Dessert::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);

		//	}

		//}

		if (Dessert::Input::IsMouseButtonPressed(DGE_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Dessert::Input::GetMousePosition();
			auto width = Dessert::Application::Get().GetWindow().GetWidth();
			auto height = Dessert::Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().getTransformPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particles.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 50; i++)
				m_ParticleSystem.Emit(m_Particles);
		}

		m_ParticleSystem.OnUpdate(delta);
		m_ParticleSystem.OnRender();

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

	auto stats = Dessert::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats: ");
	ImGui::Text("Draw Calls : %d", stats.DrawCalls);
	ImGui::Text("Quads count : %d", stats.QuadCount);
	ImGui::Text("Vertices count : %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices count : %d", stats.GetTotalIndexCount());
	ImGui::Text("Delta Time : %f ms ( %d FPS)", m_LastDeltaTime, (int)(1.0f / m_LastDeltaTime));
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
