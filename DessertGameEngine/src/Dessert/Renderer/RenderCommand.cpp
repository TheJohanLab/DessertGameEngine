#include "dgepch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Dessert {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}