//
// Created by sebsn on 29-04-2024.
//

#include "OpenGLContext.h"

namespace Kraken {
    OpenGLContext::OpenGLContext(GLFWwindow *windowHandle) : m_WindowHandle(windowHandle) {
        
    }

    void OpenGLContext::Init() {
        KR_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            KRC_CRITICAL("ERR::GL::Failed to load GLAD");
            KRC_ASSERT(false, "Failed to initializee GLAD");
        }

        KRC_INFO("OpenGL Info:");
        KRC_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        KRC_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        KRC_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        
        //KRC_INFO(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Kraken requires at least OpenGL version 4.5!");
    }
} // Kraken