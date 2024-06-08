//
// Created by sebsn on 29-04-2024.
//
#pragma once
#include <krpch.h>

#include "Framebuffer.h"
#include "Kraken/Graphics/VertexArray.h"
#include "Kraken/Graphics/Shader.h"
#include "Kraken/Graphics/UniformBuffer.h"
#include "Kraken/Graphics/Texture.h"

namespace Kraken {
    class GraphicsAPI {
    public:
        enum class API {
            None = 0, OpenGL = 1
        };
    public:
        static API GetAPI() { return s_API; }
        static Scope<GraphicsAPI> Create();
    public:
        virtual ~GraphicsAPI() = default;
        virtual void Init() = 0;

        virtual void SetClearColor(Color color) = 0;
		virtual void Clear() = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

        virtual Ref<VertexArray> CreateVertexArray() = 0;
        virtual Ref<VertexBuffer> CreateVertexBuffer(const float* vertices, uint32_t size) = 0;
        virtual Ref<VertexBuffer> CreateVertexBuffer(uint32_t size) = 0;
		virtual Ref<IndexBuffer> CreateIndexBuffer(const uint32_t *indices, uint32_t count) = 0;
		virtual Ref<Framebuffer> CreateFramebuffer(const FramebufferSpecification& spec) = 0;
        virtual Ref<Shader> CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) = 0;
        virtual Ref<Shader> CreateShader(AssetSpecification& specs) = 0;
        virtual Ref<UniformBuffer> CreateUniformBuffer(uint32_t size, uint32_t binding) = 0;
        virtual Ref<Texture2D> CreateTexture2D(AssetSpecification& specs) = 0;
        virtual Ref<Texture2D> CreateTexture2D(TextureSpecification& specs) = 0;

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

    private:
        static API s_API;
    };
}
