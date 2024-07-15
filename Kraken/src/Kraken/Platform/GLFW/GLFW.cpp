//
// Created by sebsn on 22-04-2024.
//

// ReSharper disable CppUseStructuredBinding
#include "GLFW.h"

#include "Kraken/Core/Application.h"
#include "Kraken/Events/ApplicationEvents.h"
#include "Kraken/Events/KeyEvents.h"
#include "Kraken/Events/MouseEvents.h"

namespace Kraken {
	static uint8_t s_GLFWWindowCount = 0;

    GLFWWindow::GLFWWindow(const WindowSpecs& windowSpecs) {
        KR_PROFILE_FUNCTION();

        if(s_GLFWWindowCount++ == 0) {
            KRC_TRACE("GLFWWindowCount was 0");
            if(!GLFW::InitGlfw()) {
                KRC_ASSERT(false, "GLFW Should be able to initialize")
                KRC_INFO("Stopping Window::Window");
                return;
            }
        }

        // Window Hints
        #ifdef KR_SUPPORT_OPENGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef KR_PLATFORM_OSX
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        #endif

        // Window Hints pt. 2
        glfwWindowHint(GLFW_RESIZABLE, windowSpecs.noResize ? GLFW_FALSE : GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, windowSpecs.initializeHidden ? GLFW_FALSE : GLFW_TRUE);

        // Window state
        m_State.Title = (windowSpecs.title.empty()) ? Application::GetInstance().GetApplicationInfo().Name : windowSpecs.title;
        m_State.WidthFramebuffer = windowSpecs.Width;
        m_State.HeightFrameBuffer = windowSpecs.Height;

        // Create window
        KRC_INFO("GLFW: Creating Window, Name: {}, Fullscreen {}, NoResize {}, Size {}x{}", m_State.Title, windowSpecs.initializeFullscreen, windowSpecs.noResize, windowSpecs.Width, windowSpecs.Height);
        m_Window = glfwCreateWindow(static_cast<int>(windowSpecs.Width), static_cast<int>(windowSpecs.Height), m_State.Title.c_str(), nullptr, nullptr);

        if(m_Window == nullptr) {
            const char* errorDescription;
            KRC_CRITICAL("ERR::IO::CREATE_WINDOW_FAILED, {}, {}", glfwGetError(&errorDescription), errorDescription);
            return;
        }

        glfwSetWindowUserPointer(m_Window, &m_State);

        // Callbacks
        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            KRC_TRACE("GLFW frameBufferSizeCallback: Width {} Height {}", width, height);
            WindowState& state = *static_cast<WindowState *>(glfwGetWindowUserPointer(window));
            state.WidthFramebuffer = width;
            state.HeightFrameBuffer = height;

            state.EventCallback(new WindowResizeEvent(width, height));
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            const WindowState& state = *static_cast<WindowState *>(glfwGetWindowUserPointer(window));
            state.EventCallback(new WindowCloseEvent);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            const WindowState& state = *static_cast<WindowState *>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    state.EventCallback(new KeyPressedEvent(key, false));
                    break;
                }
                case GLFW_RELEASE: {
                    state.EventCallback(new KeyReleasedEvent(key));
                    break;
                }
                case GLFW_REPEAT: {
                    state.EventCallback(new KeyPressedEvent(key, true));
                    break;
                }
                default: KRC_WARN("Unkown key action: " + action);
            }
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
            WindowState& state = *static_cast<WindowState *>(glfwGetWindowUserPointer(window));
            const float dx = static_cast<float>(xPos) - state.MouseX;
            const float dy = static_cast<float>(yPos) - state.MouseY;
            state.MouseX = static_cast<float>(xPos);
            state.MouseY = static_cast<float>(yPos);
            state.EventCallback(new MouseMovedEvent(state.MouseX, state.MouseY, dx, dy));
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
            const WindowState& state = *static_cast<WindowState *>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    state.EventCallback(new ButtonPressedEvent(button));
                    break;
                }
                case GLFW_RELEASE: {
                    state.EventCallback(new ButtonReleasedEvent(button));
                    break;
                }
                default: KRC_WARN("Unkown mouse action: " + action);
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
            const WindowState& state = *static_cast<WindowState *>(glfwGetWindowUserPointer(window));
            state.EventCallback(new MouseScrolledEvent(static_cast<float>(xOffset), static_cast<float>(yOffset)));
        });

        if(windowSpecs.initializeFullscreen) GLFWWindow::Fullscreen(true);

        // GraphicsContext
        m_GraphicsContext = GraphicsContext::Create(windowSpecs.appInfo, m_Window);
        m_GraphicsContext->Init();
    }

    void GLFWWindow::PollEvents() {
        KR_PROFILE_FUNCTION();

        glfwPollEvents();
    }

    void GLFWWindow::SwapBuffers() {
        KR_PROFILE_FUNCTION();

        glfwSwapBuffers(m_Window);
    }

    void GLFWWindow::Show() {
        KR_PROFILE_FUNCTION();

        glfwShowWindow(m_Window);
    }

    GLFWWindow::~GLFWWindow() {
        KR_PROFILE_FUNCTION();

        glfwDestroyWindow(m_Window);

        if(++s_GLFWWindowCount == 0) {
            KRC_TRACE("GLFWWindowCount was 0");
            GLFW::Terminate();
        }
    }


    void GLFWWindow::Fullscreen(const bool fullscreen) {
        KR_PROFILE_FUNCTION();

        GLFWmonitor* monitor = glfwGetWindowMonitor(m_Window);
        if((monitor != nullptr) == fullscreen) return;
        m_State.Fullscreen = fullscreen;

        if(fullscreen) {
            monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);

            glfwGetWindowPos(m_Window, &m_storedX, &m_storedY);
            glfwGetWindowSize(m_Window, &m_storedW, &m_storedH);
            glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width,mode->height,mode->refreshRate);
            KRC_TRACE("GLFW: Window changed to fullscreen");
        } else {
            glfwSetWindowMonitor(m_Window, nullptr, m_storedX, m_storedY, m_storedW, m_storedH, 0);
            KRC_TRACE("GLFW: Window changed to windowed");
        }
    }

    void error_callback(int code, const char* description) {
        KRC_ERROR("GLFW ({}): {}", code, description);
    }

    bool GLFW::InitGlfw() {
        KR_PROFILE_FUNCTION();

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
