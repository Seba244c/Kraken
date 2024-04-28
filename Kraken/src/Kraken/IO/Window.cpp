//
// Created by sebsn on 28-04-2024.
//

#include "Window.h"

#include "Kraken/Platform/GLFW/GLFW.h"

namespace Kraken {
    Scope<Window> Window::Create(const WindowSpecs &specs) {
        return CreateScope<GLFWWindow>(specs);
    }
}
