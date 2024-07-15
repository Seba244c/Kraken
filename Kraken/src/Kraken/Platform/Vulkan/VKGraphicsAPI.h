//
// Created by Sebastian Snoer on 15/07/2024.
//

#pragma once

#include <krpch.h>
#include "Kraken/Graphics/GraphicsAPI.h"

namespace Kraken {
    class VKGraphicsAPI final : public GraphicsAPI {
        void Init() override;

        void SetClearColor(Color color) override;
        void Clear() override;
        void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        Ref<VertexArray> CreateVertexArray() override;
        Ref<VertexBuffer> CreateVertexBuffer(const float *vertices, uint32_t size) override;
        Ref<VertexBuffer> CreateVertexBuffer(uint32_t size) override;
        Ref<IndexBuffer> CreateIndexBuffer(const uint32_t *indices, uint32_t count) override;
        Ref<Shader> CreateShader(const std::string &vertexSrc, const std::string &fragmentSrc) override;
        Ref<Shader> CreateShader(AssetSpecification& specs) override;
        Ref<Framebuffer> CreateFramebuffer(const FramebufferSpecification &spec) override;
        Ref<UniformBuffer> CreateUniformBuffer(uint32_t size, uint32_t binding) override;
        Ref<Texture2D> CreateTexture2D(AssetSpecification& specs) override;
        Ref<Texture2D> CreateTexture2D(TextureSpecification& specs) override;
    };
} // Kraken
