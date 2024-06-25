#include "dgepch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Dessert {
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::None:		DGE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(filepath);

			//#if DGE_PLATFORM_WINDOWS
						//case RendererAPI::Direct3D: return new Direct3DVertexBuffer(vertices, size);
			//#endif
		}

		DGE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
	
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::None:		DGE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);

			//#if DGE_PLATFORM_WINDOWS
						//case RendererAPI::Direct3D: return new Direct3DVertexBuffer(vertices, size);
			//#endif
		}

		DGE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
		
	}
	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		DGE_CORE_ASSERT(!Exists(name), "Shader already exist");
		m_Shaders[name] = shader;
	}


	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) 
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		DGE_CORE_ASSERT(Exists(name), "Shader not found");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}