#pragma once

#include "glm/gtc/matrix_transform.hpp"

namespace Dessert {

	class Camera
	{
	public:
		typedef struct Transform {

			glm::vec3 Position;
			glm::vec3 Rotation;

		}Transform;

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_VPMatrix;

		Transform m_Transform;

	public:
		Camera();
		Camera(float left, float right, float bottom, float top);
		~Camera();
		

		void calculateViewMatrix();

		void setTransformPosition(const glm::vec3& position);
		void setTransformRotation(const glm::vec3& rotation);
		void setTransform(const glm::vec3& position, const glm::vec3& rotation);

		const glm::vec3 getTransformPosition() const { return m_Transform.Position; }
		const glm::vec3 getTransformRotation() const { return m_Transform.Rotation; }

		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetVPMatrix() const { return m_VPMatrix; }
	};
}