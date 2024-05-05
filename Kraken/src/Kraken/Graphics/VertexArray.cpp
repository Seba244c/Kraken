//
// Created by sebsn on 05-05-2024.
//

#include "VertexArray.h"

#include "Renderer.h"
#include "Kraken/Platform/OpenGL/OpenGLVertexArray.h"

namespace Kraken {
    Ref<VertexArray> VertexArray::Create() {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:    KRC_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:   return CreateRef<OpenGLVertexArray>();
        }

        KRC_ASSERT(false, "Unkown API");
        return nullptr;
    }
} // Kraken