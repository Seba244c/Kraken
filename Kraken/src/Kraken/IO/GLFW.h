//
// Created by sebsn on 22-04-2024.
//

#ifndef KR_GLFW_H
#define KR_GLFW_H

#include "GLFW/glfw3.h"
#include "Kraken/Events/Event.h"

namespace Kraken {
    struct WindowSpecs {
        bool noResize = false;
        bool initializeFullscreen = false;
        bool initializeHidden = false;
        int Width = 800;
        int Height = 600;
        std::string title;
    };
    
    class Window {
    public:
        explicit Window(const WindowSpecs& windowSpecs);
        ~Window();
        
        bool ShouldClose();

        using EventCallbackFn = std::function<void(Event*)>;
        
        void SetEventCallback(const EventCallbackFn& callback) { m_State.EventCallback = callback; }
        void PollEvents();
        void SwapBuffers();
        void Show();
        void Fullscreen(bool fullscreen);

        [[nodiscard]] unsigned int GetFBWidth() const { return m_State.WidthFramebuffer; }
        [[nodiscard]] unsigned int GetFBHeight() const { return m_State.HeightFrameBuffer; }
        [[nodiscard]] bool GetFullscreen() const { return m_State.Fullscreen; }
        
        [[nodiscard]] GLFWwindow* GetNativeWindow() const { return m_Window; }
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
