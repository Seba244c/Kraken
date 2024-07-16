//
// Created by sebsn on 02-05-2024.
//

#include "GraphicsAPI.h"

#ifdef KR_SUPPORT_OPENGL
#include "Kraken/Platform/OpenGL/OpenGLGraphicsAPI.h"
#endif
#ifdef KR_SUPPORT_VULKAN
#include "Kraken/Platform/Vulkan/VKGraphicsAPI.h"
#endif

namespace Kraken {
    #ifdef KR_SUPPORT_OPENGL
        GraphicsAPI::API GraphicsAPI::s_API = GraphicsAPI::API::OpenGL;
    #else
    #ifdef KR_SUPPORT_VULKAN
        GraphicsAPI::API GraphicsAPI::s_API = GraphicsAPI::API::Vulkan;
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
#ifdef KR_SUPPORT_VULKAN
            case GraphicsAPI::API::Vulkan:    return CreateScope<VKGraphicsAPI>();
#endif
        }

        KRC_ASSERT(false, "Unkown API");
        return nullptr;
    }
}
