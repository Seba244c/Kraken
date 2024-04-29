//
// Created by sebsn on 29-04-2024.
//

#include "GraphicsContext.h"

#include "Renderer.h"
#include "GLFW/glfw3.h"
#include "Kraken/Platform/OpenGL/OpenGLContext.h"

namespace Kraken {
    Scope<GraphicsContext> GraphicsContext::Create(void *window) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:    KRC_ASSERT(false, "ERR::RENDERAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:   return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }

        KRC_ASSERT(false, "ERR::RENDERAPI::Unkown");
        return nullptr;
    }
} // Kraken