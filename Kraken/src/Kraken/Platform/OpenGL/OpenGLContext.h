//
// Created by sebsn on 29-04-2024.
//

#pragma once

#include "Kraken/Graphics/GraphicsContext.h"
struct GLFWwindow; //Just for simplicitys sake

namespace Kraken {
    class OpenGLContext final : public GraphicsContext {
    public:
        explicit OpenGLContext(GLFWwindow* windowHandle);
        
        void Init() override;
        void WaitForFrame() override;
        void NewFrame() override;
        void ShowFrame() override;
        void EnsureIdle() override;

    private:
        GLFWwindow* m_WindowHandle;
    };
} // Kraken
