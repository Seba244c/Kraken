//
// Created by sebsn on 02-05-2024.
//

#include "GraphicsAPI.h"

#ifdef KR_SUPPORT_OPENGL
#include "Kraken/Platform/OpenGL/OpenGLGraphicsAPI.h"
#endif

namespace Kraken {
    #ifdef KR_SUPPORT_OPENGL
        GraphicsAPI::API GraphicsAPI::s_API = GraphicsAPI::API::OpenGL;
    #else
        #error No GraphicsAPI is currently supported on this platform
    #endif

    Scope<GraphicsAPI> GraphicsAPI::Create() {
        switch (s_API) {
            case GraphicsAPI::API::None:    KRC_ASSERT(false, "GraphicsAPI::None is currently not supported!"); return nullptr;
#ifdef KR_SUPPORT_OPENGL
            case GraphicsAPI::API::OpenGL:   return CreateScope<OpenGLGraphicsAPI>();
#endif
        }

        KRC_ASSERT(false, "Unkown API");
        return nullptr;
    }
}
