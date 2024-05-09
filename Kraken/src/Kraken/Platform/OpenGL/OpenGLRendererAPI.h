//
// Created by sebsn on 02-05-2024.
//

#pragma once
#include <krpch.h>

#include "Kraken/Graphics/RendererAPI.h"
#include "Kraken/Platform/OpenGL/OpenGLVertexArray.h"

namespace Kraken {
    class OpenGLRendererAPI final : public RendererAPI {
    public:
        void Init() override;

        void SetClearColor(Color color) override;
        void Clear() override;
        void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        Ref<VertexArray> CreateVertexArray() override;
        Ref<VertexBuffer> CreateVertexBuffer(const float *vertices, uint32_t size) override;
        Ref<IndexBuffer> CreateIndexBuffer(const uint32_t *indices, uint32_t count) override;
        Ref<Shader> CreateShader(const std::string &vertexSrc, const std::string &fragmentSrc) override;
        Ref<Framebuffer> CreateFramebuffer(const FramebufferSpecification &spec) override;
    };
} // Kraken
