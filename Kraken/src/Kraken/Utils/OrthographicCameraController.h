#pragma once

#include <krpch.h>
#include "Kraken/Events/Event.h"
#include "Kraken/Events/ApplicationEvents.h"
#include "Kraken/Events/MouseEvents.h"
#include "Kraken/Graphics/Camera.h"
#include "Kraken/Platform/Time.h"

namespace Kraken {
	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio); // aspect-ratio * 2 units

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		void OnResize(float width, float height);

		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(const WindowResizeEvent& e);
	public:
		float CameraSpeed = 6.0f;
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = glm::vec3{ 0.0f };
	};
}
