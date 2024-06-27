#include "dgepch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Dessert {
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		{
			switch (Renderer::GetRendererAPI())
			{
			case RendererAPI::API::None:		DGE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLTexture2D>(width, height);

				//#if DGE_PLATFORM_WINDOWS
							//case RendererAPI::Direct3D: return new Direct3DVertexBuffer(vertices, size);
				//#endif
			}

			DGE_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}


	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::None:		DGE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreateRef<OpenGLTexture2D>(path);

			//#if DGE_PLATFORM_WINDOWS
						//case RendererAPI::Direct3D: return new Direct3DVertexBuffer(vertices, size);
			//#endif
		}

		DGE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}


	
