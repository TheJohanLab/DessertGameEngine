#include "dgepch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

//#if DGE_PLATFORM_WINDOWS
//#include "Platform/Direct3D/Direct3DBuffer.h"
//#endif

namespace Dessert {

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetRendererAPI())
		{
			case RendererAPI::API::None:		DGE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLVertexBuffer>(vertices, size);

//#if DGE_PLATFORM_WINDOWS
			//case RendererAPI::Direct3D: return new Direct3DVertexBuffer(vertices, size);
//#endif
		}

		DGE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::None:		DGE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreateRef<OpenGLVertexBuffer>(size);

			//#if DGE_PLATFORM_WINDOWS
						//case RendererAPI::Direct3D: return new Direct3DVertexBuffer(vertices, size);
			//#endif
		}

		DGE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::None:		DGE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreateRef<OpenGLIndexBuffer>(indices, count);

//#if DGE_PLATFORM_WINDOWS
		//case RendererAPI::Direct3D: return new Direct3DVertexBuffer(vertices, size);
//#endif
		}

		DGE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}