//
// Created by sebsn on 07-05-2024.
//

#pragma once
#include <krpch.h>

#include "GraphicsAPI.h"

namespace Kraken {
    class RenderCommand {
    public:
        static void Init() {
            s_GraphicsAPI->Init();
        }

        static void SetClearColor(const Color color) {
            s_GraphicsAPI->SetClearColor(color);
        }

        static void SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) {
            s_GraphicsAPI->SetViewport(x, y, width, height);
        }

        static void Clear() {
            s_GraphicsAPI->Clear();
        }

        static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) {
            s_GraphicsAPI->DrawIndexed(vertexArray, indexCount);
        }

        static Ref<VertexArray> CreateVertexArray() {
            return s_GraphicsAPI->CreateVertexArray();
        }

        static Ref<VertexBuffer> CreateVertexBuffer(const float* vertices, const uint32_t size) {
            return s_GraphicsAPI->CreateVertexBuffer(vertices, size);
        }

        static Ref<VertexBuffer> CreateVertexBuffer(const uint32_t size) {
            return s_GraphicsAPI->CreateVertexBuffer(size);
        }

        static Ref<IndexBuffer> CreateIndexBuffer(const uint32_t *indices, const uint32_t count) {
            return s_GraphicsAPI->CreateIndexBuffer(indices, count);
        }

        static Ref<Shader> CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) {
            return s_GraphicsAPI->CreateShader(vertexSrc, fragmentSrc);
        }

        static Ref<Shader> CreateShader(AssetSpecification& specs) {
            return s_GraphicsAPI->CreateShader(specs);
        }

        static Ref<UniformBuffer> CreateUniformBuffer(size_t size, int binding) {
            return s_GraphicsAPI->CreateUniformBuffer(size, binding);
        }

		static Ref<Texture2D> CreateTexture(AssetSpecification& assetSpecification) {
	        return s_GraphicsAPI->CreateTexture2D(assetSpecification);
        }

		static Ref<Texture2D> CreateTexture(TextureSpecification textureSpecification) {
	        return s_GraphicsAPI->CreateTexture2D(textureSpecification);
        }

    private:
        static Scope<GraphicsAPI> s_GraphicsAPI;
    };
}
