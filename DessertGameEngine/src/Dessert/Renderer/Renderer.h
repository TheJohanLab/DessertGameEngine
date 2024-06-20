#pragma once

#include "RenderCommand.h"

namespace Dessert{

	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetRendererAPI() { return RendererAPI::GetAPI(); }
	
	};
}