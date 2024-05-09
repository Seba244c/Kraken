//
// Created by sebsn on 09-05-2024.
//

#pragma once
#include <krpch.h>

#include "Framebuffer.h"

namespace Kraken {
    class Camera {
    public:
        Camera() = default;
        explicit Camera(const glm::mat4& projection) : m_Projection(projection) {}

        ~Camera() = default;

        [[nodiscard]] const glm::mat4& GetProjection() const { return m_Projection; }
        [[nodiscard]] const Scope<Framebuffer>& GetFBO() const { return m_Framebuffer; }
    protected:
        glm::mat4 m_Projection = glm::mat4(1.0f);
        Scope<Framebuffer> m_Framebuffer;
    };
}
