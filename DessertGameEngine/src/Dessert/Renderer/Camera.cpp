#include "dgepch.h"
#include "Camera.h"

namespace Dessert {

	Camera::Camera()
		:m_ProjectionMatrix(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f)),
		m_ViewMatrix(glm::mat4(1.0f)),
		m_Transform({ {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}, {1.0f, 1.0f, 1.0f} })
	{
		DGE_PROFILE_FUNCTION();

		calculateViewMatrix();
	}

	Camera::Camera(float left, float right, float bottom, float top)
		:m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
		m_ViewMatrix(glm::mat4(1.0f)),
		m_Transform({ {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}, {1.0f, 1.0f, 1.0f}  })
	{
		DGE_PROFILE_FUNCTION();

		//calculateViewMatrix();
		m_VPMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	Camera::~Camera()
	{
	}

	void Camera::setProjection(float left, float right, float bottom, float top)
	{
		DGE_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_VPMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::calculateViewMatrix()
	{
		DGE_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Transform.Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Transform.Rotation.z), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), m_Transform.Scale);

		m_ViewMatrix = glm::inverse(transform);
		m_VPMatrix = m_ProjectionMatrix * m_ViewMatrix ; // This order is for OpenGL, don't work for V * P in this case
	}

	void Camera::setTransformPosition(const glm::vec3& position)
	{
		m_Transform.Position = position;
		calculateViewMatrix();
	}

	void Camera::setTransformRotation(const glm::vec3& rotation)
	{
		m_Transform.Rotation = rotation;
		calculateViewMatrix();
	}
	void Camera::setTransformScale(const glm::vec3& scale)
	{
		m_Transform.Scale = scale;
		calculateViewMatrix();
	}

	void Camera::setTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	{
		m_Transform.Position = position;
		m_Transform.Rotation = rotation;
		m_Transform.Scale = scale;
		calculateViewMatrix();
	}
	
}