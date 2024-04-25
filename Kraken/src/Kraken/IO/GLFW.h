//
// Created by sebsn on 22-04-2024.
//

#ifndef KR_GLFW_H
#define KR_GLFW_H
#include "GLFW/glfw3.h"

namespace Kraken {
    struct WindowSpecs {
        bool noResize = false;
        bool initializeFullscreen = false;
        int width = 800;
        int height = 600;
        const char* title = nullptr;
    };
    
    class Window {
    public:
        explicit Window(const WindowSpecs& windowSpecs);
        ~Window();
        bool ShouldClose();
        void PollEvents();
    private:
        GLFWwindow* window_;
    };
    
    class GLFW {
    public:
        static bool InitGlfw();
        static void Terminate();
    };
}

#endif //KR_GLFW_H
