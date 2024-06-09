//
// Created by sebsn on 29-04-2024.
//

#ifndef KR_OPENGLCONTEXT_H
#define KR_OPENGLCONTEXT_H

#include "Kraken/Graphics/GraphicsContext.h"
struct GLFWwindow; //Just for simplicitys sake

namespace Kraken {
    class OpenGLContext final : public GraphicsContext {
    public:
        explicit OpenGLContext(GLFWwindow* windowHandle);
        
        void Init() override;
    private:
        GLFWwindow* m_WindowHandle;
    };
} // Kraken

#endif //KR_OPENGLCONTEXT_H
