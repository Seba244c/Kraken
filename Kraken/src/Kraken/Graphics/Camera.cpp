//
// Created by sebsn on 09-05-2024.
//

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Kraken {
    OrthographicCamera::OrthographicCamera(const float left, const float right, const float bottom, const float top)
        : Camera(glm::ortho(left, right, bottom, top, -1.0f, 1.0f), 1.0f) {}

    void OrthographicCamera::RecalculateViewMatrix() {
        const auto transform = translate(glm::mat4(1.0f), m_Position)
                               * rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0,0,1)); // Rotation only along z axis, since this is a orthographic camera
        m_ViewMatrix = inverse(transform); // For cameras
        m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
    }
}
