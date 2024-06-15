#include <Dessert.h>

#include "imgui/imgui.h"



class ExampleLayer : public Dessert::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{
	
	}

	void OnUpdate() override
	{
		//DGE_INFO("ExampleLayer::Update");

		if (Dessert::Input::isKeyPressed(DGE_KEY_TAB))
			DGE_TRACE("TAB key is pressed");
	}

	void OnEvent(Dessert::Event& event) override
	{
		if (event.GetEventType() == Dessert::EventType::KeyPressed)
		{
			Dessert::KeyPressedEvent& e = (Dessert::KeyPressedEvent&)event;
			DGE_TRACE("{0}", (char)e.getKeyCode());
		}
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello world");
		ImGui::ColorEdit4("", new float[4]);
		ImGui::End();
	}
};

class Sandbox : public Dessert::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Dessert::Application* Dessert::CreateApplication()
{
	return new Sandbox();
}
