#pragma once

#include "Dessert/Renderer/Shader.h"

#include "glm/glm.hpp"

// TODO remove
typedef unsigned int GLenum;

namespace Dessert {

	class OpenGLShader: public Shader
	{
	private:
		uint32_t m_RendererId;

	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void setUniformMat4(const char* name, const glm::mat4& matrix) const;

		void setUniformInt(const char* name, int value) const;

		void setUniformFloat(const char* name, float value) const;
		void setUniformFloat2(const char* name, const glm::vec2& values) const;
		void setUniformFloat3(const char* name, const glm::vec3& values) const;
		void setUniformFloat4(const char* name, const glm::vec4& values) const;

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	
	};
}