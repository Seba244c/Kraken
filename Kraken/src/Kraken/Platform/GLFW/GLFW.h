//
// Created by sebsn on 22-04-2024.
//

#pragma once
#include <krpch.h>

#include "Kraken/IO/Window.h"
#include "Kraken/Graphics/GraphicsContext.h"

namespace Kraken {
    class GLFWWindow final : public Window {
    public:
        explicit GLFWWindow(const WindowSpecs& windowSpecs);
		~GLFWWindow() override;

        void PollEvents() override;
        void Show() override;
        void Fullscreen(bool fullscreen) override;

        void SetEventCallback(const EventCallbackFn& callback) override { m_State.EventCallback = callback; }
        [[nodiscard]] uint32_t GetFBWidth() const override { return m_State.WidthFramebuffer; };
        [[nodiscard]] uint32_t GetFBHeight() const override { return m_State.HeightFrameBuffer; };
        [[nodiscard]] bool IsFullscreen() const override { return m_State.Fullscreen; };
        [[nodiscard]] void * GetNativeWindow() const override { return m_Window; }

        [[nodiscard]] const Scope<GraphicsContext> &GetGraphicsContext() override { return m_GraphicsContext; }

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
        Scope<GraphicsContext> m_GraphicsContext;
    };
    
    class GLFW {
    public:
        static bool InitGlfw();
        static void Terminate();
    };
}