//
// Created by sebsn on 02-05-2024.
//

#include "GraphicsAPI.h"

#ifdef KR_SUPPORT_OPENGL
#include "Kraken/Platform/OpenGL/OpenGLGraphicsAPI.h"
#endif
#ifdef KR_SUPPORT_METAL
#include "Kraken/Platform/Metal/MetalGraphicsAPI.h"
#endif

namespace Kraken {
    #ifdef KR_SUPPORT_OPENGL
        GraphicsAPI::API GraphicsAPI::s_API = GraphicsAPI::API::OpenGL;
    #else
    #ifdef KR_SUPPORT_METAL
        GraphicsAPI::API GraphicsAPI::s_API = GraphicsAPI::API::Metal;
#else
        #error No GraphicsAPI is currently supported on this platform
    #endif
    #endif

    Scope<GraphicsAPI> GraphicsAPI::Create() {
        switch (s_API) {
            case GraphicsAPI::API::None:    KRC_ASSERT(false, "GraphicsAPI::None is currently not supported!"); return nullptr;
#ifdef KR_SUPPORT_OPENGL
            case GraphicsAPI::API::OpenGL:   return CreateScope<OpenGLGraphicsAPI>();
#endif
#ifdef KR_SUPPORT_METAL
            case GraphicsAPI::API::Metal:    return CreateScope<MetalGraphicsAPI>();
#endif
        }

        KRC_ASSERT(false, "Unkown API");
        return nullptr;
    }
}
