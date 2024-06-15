//
// Created by sebsn on 09-05-2024.
//

#pragma once
#include <krpch.h>

#include "Framebuffer.h"

namespace Kraken {
    class Camera {
    public:
        Camera(const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix);

        ~Camera() = default;

        [[nodiscard]] const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        [[nodiscard]] const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        [[nodiscard]] const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
        [[nodiscard]] const Scope<Framebuffer>& GetFBO() const { return m_Framebuffer; }
        void SetFBO(Scope<Framebuffer>& framebuffer) { m_Framebuffer = std::move(framebuffer); }
    protected:
        glm::mat4 m_ProjectionMatrix;
    	glm::mat4 m_ViewMatrix;
    	glm::mat4 m_ViewProjectionMatrix;
        Scope<Framebuffer> m_Framebuffer;
    };

    class OrthographicCamera : public Camera {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        // For modifying ProjectionMatrix
    	void SetProjection(float left, float right, float bottom, float top);

        // For modifying ViewMatrix
        [[nodiscard]] const glm::vec3& GetPosition() const { return m_Position; }
        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

        [[nodiscard]] float GetRotation() const { return m_Rotation; }
        void SetRotation(const float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
    private:
        void RecalculateViewMatrix();
    private:
        glm::vec3 m_Position = glm::vec3(0.0f);

        // Rotation around the z axis
        float m_Rotation = 0.0f;
    };
}
