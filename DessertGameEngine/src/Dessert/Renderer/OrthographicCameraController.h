#pragma once

#include "Dessert/Renderer/Camera.h"

#include "Dessert/Core/Timestep.h"
#include "Dessert/Events/Event.h"

#include "Dessert/Events/ApplicationEvent.h"
#include "Dessert/Events/MouseEvent.h"

namespace Dessert {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0;
		OrthographicCameraBounds m_Bounds;
		Camera m_Camera;

		bool m_Rotation;

		float m_CameraRotation = 0.0f;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f , 0.0f };
		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 90.0f;


	public:
		OrthographicCameraController(float aspectRatio,  bool rotation = false);

		void OnUpdate(Timestep delta);
		void OnEvent(Event& e);

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		float GetZoomLevel() const { return m_ZoomLevel; }

		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	};
}