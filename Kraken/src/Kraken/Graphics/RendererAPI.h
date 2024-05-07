//
// Created by sebsn on 29-04-2024.
//
#pragma once
#include <krpch.h>

#include "Kraken/Graphics/VertexArray.h"
#include "Kraken/Graphics/Shader.h"

namespace Kraken {
    class RendererAPI {
    public:
        enum class API {
            None = 0, OpenGL = 1
        };
    public:
        static API GetAPI() { return s_API; }
        static Scope<RendererAPI> Create();
    public:
        virtual ~RendererAPI() = default;
        virtual void Init() = 0;

        virtual void SetClearColor(Color color) = 0;
		virtual void Clear() = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

        virtual Ref<VertexArray> CreateVertexArray() = 0;
        virtual Ref<VertexBuffer> CreateVertexBuffer(const float* vertices, uint32_t size) = 0;
		virtual Ref<IndexBuffer> CreateIndexBuffer(const uint32_t *indices, uint32_t count) = 0;
        virtual Ref<Shader> CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) = 0;
    private:
        static API s_API;
    };
}
