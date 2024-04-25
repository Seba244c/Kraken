//
// Created by sebsn on 22-04-2024.
//

#include "GLFW.h"

#include "Kraken/Core/Application.h"

namespace Kraken {
	static uint8_t s_GLFWWindowCount = 0;
    
    Window::Window(const WindowSpecs& windowSpecs) {
        if(s_GLFWWindowCount++ == 0) {
            KRC_TRACE("GLFWWindowCount was 0");
            if(!GLFW::InitGlfw()) {
                KRC_ASSERT(false, "GLFW Should be able to initialize")
                KRC_INFO("Stopping Window::Window");
                return;
            }
        }
        
        // Window Hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #ifdef KR_PLATFORM_OSX
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        // Window options
        glfwWindowHint(GLFW_RESIZABLE, windowSpecs.noResize ? GL_FALSE : GL_TRUE);

        // Create window
        auto title = (windowSpecs.title == nullptr) ? Application::GetInstance().GetApplicationInfo().Name.c_str() : windowSpecs.title;
        KRC_INFO("GLFW: Creating Window, Name: {}, Fullscreen {}, NoResize {}, Size {}x{}", title, windowSpecs.initializeFullscreen, windowSpecs.noResize, windowSpecs.width, windowSpecs.height);
        window_ = glfwCreateWindow(windowSpecs.width, windowSpecs.height, title, nullptr, nullptr);
        
        if(window_ == nullptr) {
            const char* errorDescription;
            KRC_CRITICAL("ERR::IO::CREATE_WINDOW_FAILED, {}, {}", glfwGetError(&errorDescription), errorDescription);
            return;
        }

        glfwMakeContextCurrent(window_);
        glfwShowWindow(window_);
    }

    Window::~Window() {
        glfwDestroyWindow(window_);
        
        if(++s_GLFWWindowCount == 0) {
            KRC_TRACE("GLFWWindowCount was 0");
            GLFW::Terminate();
        }
    }

    bool Window::ShouldClose() {
        return glfwWindowShouldClose(window_) == GLFW_TRUE;
    }

    void Window::PollEvents() {
        glfwPollEvents();
    }

    void error_callback(int code, const char* description) {
        KRC_ERROR("GLFW ({}): {}", code, description);
    }

    bool GLFW::InitGlfw() {
        KRC_INFO("GLFW: Initializing GLFW");
        
        glfwSetErrorCallback(error_callback);
        if(glfwInit() == GLFW_FALSE) {
            const char* errorDescription;
            KRC_CRITICAL("ERR::IO::INIT_GLFW_FAILED, {}, {}", glfwGetError(&errorDescription), errorDescription);

            glfwTerminate();
            return false;
        }

        return true;
    }

    void GLFW::Terminate() {
        KRC_INFO("GLFW: Terminating");
        glfwTerminate();
    };
}
