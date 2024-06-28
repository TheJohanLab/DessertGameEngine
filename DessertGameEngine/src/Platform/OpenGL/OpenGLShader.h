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
		std::string m_Name;

	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;



		virtual void SetMat4(const char* name, const glm::mat4& value) override;
		virtual void SetInt(const char* name, int value) override;
		virtual void SetFloat(const char* name, float value) override;
		virtual void SetFloat2(const char* name, const glm::vec2& value) override;
		virtual void SetFloat3(const char* name, const glm::vec3& value) override;
		virtual void SetFloat4(const char* name, const glm::vec4& value) override;


		virtual const std::string& GetName() const override { return m_Name; }

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