#include "dgepch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Dessert {

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::None:		DGE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLTexture2D>(path);

			//#if DGE_PLATFORM_WINDOWS
						//case RendererAPI::Direct3D: return new Direct3DVertexBuffer(vertices, size);
			//#endif
		}

		DGE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}


	
