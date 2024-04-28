//
// Created by sebsn on 22-04-2024.
//

#ifndef KR_GLFW_H
#define KR_GLFW_H

#include "Kraken/IO/Window.h"
#include "GLFW/glfw3.h"
#include "Kraken/Events/Event.h"

namespace Kraken {
    class GLFWWindow : public Window {
    public:
        explicit GLFWWindow(const WindowSpecs& windowSpecs);
		~GLFWWindow() override;

        void PollEvents() override;
        void SwapBuffers() override;
        void Show() override;
        void Fullscreen(bool fullscreen) override;

        void SetEventCallback(const EventCallbackFn& callback) override { m_State.EventCallback = callback; }
        [[nodiscard]] uint32_t GetFBWidth() const override { return m_State.WidthFramebuffer; };
        [[nodiscard]] uint32_t GetFBHeight() const override { return m_State.HeightFrameBuffer; };
        [[nodiscard]] bool IsFullscreen() const override { return m_State.Fullscreen; };
        [[nodiscard]] void * GetNativeWindow() const override { return m_Window; };

    private:
        struct WindowState {
            std::string Title;
            EventCallbackFn EventCallback;
            unsigned int WidthFramebuffer, HeightFrameBuffer;
            bool Fullscreen;

            float MouseX, MouseY;
        };
        
        WindowState m_State;
        GLFWwindow* m_Window;
        int m_storedX = 0, m_storedY = 0, m_storedW = 0, m_storedH = 0; // Used to save window state before entering fullscreen
    };
    
    class GLFW {
    public:
        static bool InitGlfw();
        static void Terminate();
    };
}

#endif //KR_GLFW_H
