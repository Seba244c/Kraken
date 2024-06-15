#include "OrthographicCameraController.h"

#include "Kraken/IO/Input.h"

namespace Kraken {
	OrthographicCameraController::OrthographicCameraController(const float aspectRatio) : m_AspectRatio(aspectRatio), m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel) {

	}

	void OrthographicCameraController::OnUpdate(Timestep ts) {
        KR_PROFILE_FUNCTION();

        auto direction = glm::vec3(0.0f);
        float scaledSpeed = CameraSpeed * ts;

        if(Input::IsKeyPressed(Key::W)) direction.y += 1;
        if(Input::IsKeyPressed(Key::S)) direction.y -= 1;
        if(Input::IsKeyPressed(Key::D)) direction.x += 1;
        if(Input::IsKeyPressed(Key::A)) direction.x -= 1;

        if(length(direction) > 0) {
			m_CameraPosition += normalize(direction) * scaledSpeed;
            m_Camera.SetPosition(m_CameraPosition);
        }
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		EventDispatcher dispatcher(&e);
		dispatcher.Dispatch<MouseScrolledEvent>(KR_BIND_EVENT_FN(OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(KR_BIND_EVENT_FN(OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height) {
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
        KR_PROFILE_FUNCTION();

		m_ZoomLevel -= e.YOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(const WindowResizeEvent& e) {
        KR_PROFILE_FUNCTION();

		OnResize(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
		if(m_Camera.GetFBO()) m_Camera.GetFBO()->Resize(e.GetWidth(), e.GetHeight());

		return false;
	}
}
