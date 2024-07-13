#include <Dessert.h>
#include <Dessert/Core/EntryPoint.h>


//#include "Platform/OpenGL/OpenGLShader.h"
//
//#include "imgui/imgui.h"
//
//#include <glm/gtc/type_ptr.hpp>
#include "Sandbox2D.h"

#include "GameLayer.h"


class Sandbox : public Dessert::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
		//PushLayer(new GameLayer());
	}

	~Sandbox()
	{

	}
};

Dessert::Application* Dessert::CreateApplication()
{
	return new Sandbox();
}
