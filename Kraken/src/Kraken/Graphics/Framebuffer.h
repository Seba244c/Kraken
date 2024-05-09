//
// Created by sebsn on 09-05-2024.
//

#pragma once

#include <krpch.h>

namespace Kraken {
    struct FramebufferSpecification {
        uint32_t Width = 0, Height = 0;
        uint32_t Samples = 1;
    };

    class Framebuffer {
    public:
        virtual ~Framebuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
    };
} // Kraken
