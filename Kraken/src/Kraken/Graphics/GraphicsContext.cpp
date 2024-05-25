//
// Created by sebsn on 29-04-2024.
//

#include "GraphicsContext.h"

#include "Renderer.h"

#ifdef KR_SUPPORT_OPENGL
#include "Kraken/Platform/OpenGL/OpenGLContext.h"
#endif

namespace Kraken {
    Scope<GraphicsContext> GraphicsContext::Create(void *window) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:    KRC_ASSERT(false, "ERR::RENDERAPI::None is currently not supported!"); return nullptr;

#ifdef KR_SUPPORT_OPENGL
#ifdef KR_SUPPORT_GLFW
            case RendererAPI::API::OpenGL:   return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
#else
#error OpenGL Rendering API Does not support any of the windowing libraries supported by this platform
#endif
#endif
        }

        KRC_ASSERT(false, "ERR::RENDERAPI::Unkown");
        return nullptr;
    }
} // Kraken