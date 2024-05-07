//
// Created by sebsn on 02-05-2024.
//

#include "RendererAPI.h"

#ifdef KR_SUPPORT_OPENGL
#include "Kraken/Platform/OpenGL/OpenGLRendererAPI.h"
#endif

namespace Kraken {
    #ifdef KR_SUPPORT_OPENGL
        RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
    #else
        #error No RenderingAPI is currently supported on this platform
    #endif

    Scope<RendererAPI> RendererAPI::Create() {
        switch (s_API) {
            case RendererAPI::API::None:    KRC_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
#ifdef KR_SUPPORT_OPENGL
            case RendererAPI::API::OpenGL:   return CreateScope<OpenGLRendererAPI>();
#endif
        }

        KRC_ASSERT(false, "Unkown API");
        return nullptr;
    }
}
