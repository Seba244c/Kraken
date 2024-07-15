//
// Created by Sebastian Snoer on 15/07/2024.
//

#include "VKGraphicsAPI.h"

namespace Kraken {
    void VKGraphicsAPI::Init() {
        KR_PROFILE_FUNCTION();

    }

    void VKGraphicsAPI::SetClearColor(Color color) {
        return;
    }

    void VKGraphicsAPI::Clear() {
        return;
    }

    void VKGraphicsAPI::DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount) {
        return;
    }

    void VKGraphicsAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        return;
    }

    Ref<VertexArray> VKGraphicsAPI::CreateVertexArray() {
        return nullptr;
    }

    Ref<VertexBuffer> VKGraphicsAPI::CreateVertexBuffer(const float *vertices, uint32_t size) {
        return nullptr;
    }

    Ref<VertexBuffer> VKGraphicsAPI::CreateVertexBuffer(uint32_t size) {
        return nullptr;
    }

    Ref<IndexBuffer> VKGraphicsAPI::CreateIndexBuffer(const uint32_t *indices, uint32_t count) {
        return nullptr;
    }

    Ref<Shader> VKGraphicsAPI::CreateShader(const std::string &vertexSrc, const std::string &fragmentSrc) {
        return nullptr;
    }

    Ref<Shader> VKGraphicsAPI::CreateShader(AssetSpecification &specs) {
        return nullptr;
    }

    Ref<Framebuffer> VKGraphicsAPI::CreateFramebuffer(const FramebufferSpecification &spec) {
        return nullptr;
    }

    Ref<UniformBuffer> VKGraphicsAPI::CreateUniformBuffer(uint32_t size, uint32_t binding) {
        return nullptr;
    }

    Ref<Texture2D> VKGraphicsAPI::CreateTexture2D(AssetSpecification &specs) {
        return nullptr;
    }

    Ref<Texture2D> VKGraphicsAPI::CreateTexture2D(TextureSpecification &specs) {
        return nullptr;
    }
} // Kraken
