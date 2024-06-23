#pragma once

#include "Dessert/Renderer/Shader.h"

#include "glm/glm.hpp"


namespace Dessert {

	class OpenGLShader: public Shader
	{
	public:
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
		uint32_t m_RendererId;
	};
}