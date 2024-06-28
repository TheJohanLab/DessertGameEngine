#include "dgepch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Dessert {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):
		m_WindowHandle(windowHandle)
	{
		DGE_CORE_ASSERT(windowHandle, "Window handle is null");
	}

	void OpenGLContext::Init()
	{
		DGE_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		DGE_CORE_ASSERT(status, "Failed to initialize Glad");
	
		DGE_CORE_INFO("OpenGL Infos : ");
		DGE_CORE_INFO(" Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		DGE_CORE_INFO(" Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		DGE_CORE_INFO(" Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		DGE_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}