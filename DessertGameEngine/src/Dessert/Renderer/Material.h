#pragma once

#include "Shader.h"
#include "glm/glm.hpp"

namespace Dessert {

	class Material
	{
	private:
		Ref<Shader> m_Shader;

	public:
		Material(Shader* shader);

		void SetData(const char* name, const glm::vec3& data);
	};
}