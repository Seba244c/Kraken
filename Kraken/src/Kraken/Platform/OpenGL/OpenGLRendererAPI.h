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
        void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

        Ref<VertexArray> CreateVertexArray() override;
        Ref<VertexBuffer> CreateVertexBuffer(const float *vertices, uint32_t size) override;
        Ref<IndexBuffer> CreateIndexBuffer(const uint32_t *indices, uint32_t count) override;
        Ref<Shader> CreateShader(const std::string &vertexSrc, const std::string &fragmentSrc) override;
    };
} // Kraken
