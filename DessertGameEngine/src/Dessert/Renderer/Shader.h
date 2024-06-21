#pragma once

#include "glm/glm.hpp"
#include <string>

namespace Dessert {

	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void setUniformMat4f(const char* name, const glm::mat4& matrix) const;

	private:
		uint32_t m_RendererId;
	};
}