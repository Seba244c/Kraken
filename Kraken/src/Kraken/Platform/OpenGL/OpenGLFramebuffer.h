//
// Created by sebsn on 09-05-2024.
//

#pragma once
#include "Kraken/Graphics/Framebuffer.h"

namespace Kraken {
    class OpenGLFramebuffer final : public Framebuffer {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        ~OpenGLFramebuffer() override;

        void Bind() override;
        void Unbind() override;
    };
} // Kraken
