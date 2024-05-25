//
// Created by sebsn on 07-05-2024.
//

#pragma once
#include <krpch.h>

#include "RendererAPI.h"

namespace Kraken {
    class RenderCommand {
    public:
        static void Init() {
            s_RendererAPI->Init();
        }

        static void SetClearColor(const Color color) {
            s_RendererAPI->SetClearColor(color);
        }

        static void SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) {
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        static void Clear() {
            s_RendererAPI->Clear();
        }

        static void DrawIndexed(const Ref<VertexArray>& vertexArray) {
            s_RendererAPI->DrawIndexed(vertexArray);
        }

        static Ref<VertexArray> CreateVertexArray() {
            return s_RendererAPI->CreateVertexArray();
        }

        static Ref<VertexBuffer> CreateVertexBuffer(const float* vertices, const uint32_t size) {
            return s_RendererAPI->CreateVertexBuffer(vertices, size);
        }

        static Ref<IndexBuffer> CreateIndexBuffer(const uint32_t *indices, const uint32_t count) {
            return s_RendererAPI->CreateIndexBuffer(indices, count);
        }
        static Ref<Shader> CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) {
            return s_RendererAPI->CreateShader(vertexSrc, fragmentSrc);
        }

        static Ref<UniformBuffer> CreateUniformBuffer(size_t size, int binding) {
            return s_RendererAPI->CreateUniformBuffer(size, binding);
        }

    private:
        static Scope<RendererAPI> s_RendererAPI;
    };
}
