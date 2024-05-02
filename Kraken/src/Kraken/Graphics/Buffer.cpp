//
// Created by sebsn on 02-05-2024.
//

#include "Buffer.h"
#include "Renderer.h"

#include "Kraken/Platform/OpenGL/OpenGLBuffer.h"

namespace Kraken {
    Ref<VertexBuffer> VertexBuffer::Create(const float* vertices, uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:    KRC_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:   return CreateRef<OpenGLVertexBuffer>(vertices, size);
        }

        KRC_ASSERT(false, "Unkown API");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(const uint32_t *indices, uint32_t count) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:    KRC_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:   return CreateRef<OpenGLIndexBuffer>(indices, count);
        }

        KRC_ASSERT(false, "Unkown API");
        return nullptr;
    }
}
