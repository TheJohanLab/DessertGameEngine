#include "dgepch.h"
#include "OrthographicCameraController.h"

#include "Dessert/Core/Input.h"
#include "Dessert/Core/KeyCodes.h"


namespace Dessert {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:m_AspectRatio(aspectRatio), m_Rotation(rotation),
		m_Bounds( { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }),
		m_Camera(
			-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep delta)
	{
		DGE_PROFILE_FUNCTION();

		if (Input::isKeyPressed(DGE_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * delta;
		else if (Input::isKeyPressed(DGE_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * delta;


		if (Input::isKeyPressed(DGE_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * delta;
		else if (Input::isKeyPressed(DGE_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * delta;

		if (m_Rotation)
		{
			if (Input::isKeyPressed(DGE_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * delta;
			
			if (Input::isKeyPressed(DGE_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * delta;
			
			m_Camera.setTransformRotation(glm::vec3(0.0f, 0.0f, m_CameraRotation));
		}

		m_Camera.setTransformPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel;


	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		DGE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNC(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		DGE_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.setProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		DGE_PROFILE_FUNCTION();

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.setProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
		return false;
	}
}