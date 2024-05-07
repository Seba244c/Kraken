//
// Created by sebsn on 28-04-2024.
//

#include "Window.h"

#ifdef KR_SUPPORT_GLFW
#include "Kraken/Platform/GLFW/GLFW.h"
#endif

namespace Kraken {
    Scope<Window> Window::Create(const WindowSpecs &specs) {
#ifdef KR_SUPPORT_GLFW
        return CreateScope<GLFWWindow>(specs);
#else
        #error No windowing library is currently supported on this platform
#endif
    }
}
