//
// Created by sebsn on 02-05-2024.
//

#include "RendererAPI.h"

#include "Kraken/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Kraken {
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

    Scope<RendererAPI> RendererAPI::Create() {
        switch (s_API) {
            case RendererAPI::API::None:    KRC_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:   return CreateScope<OpenGLRendererAPI>();
        }

        KRC_ASSERT(false, "Unkown API");
        return nullptr;
    }
}
