#include "dgepch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Dessert {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::None:		DGE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLVertexArray();

			//#if DGE_PLATFORM_WINDOWS
						//case RendererAPI::Direct3D: return new Direct3DVertexBuffer(vertices, size);
			//#endif
		}

		DGE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}