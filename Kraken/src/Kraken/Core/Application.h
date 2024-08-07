//
// Created by sebsn on 21-04-2024.
//

#pragma once
#include <krpch.h>
#include "Layerstack.h"
#include "Kraken/Events/ApplicationEvents.h"
#include "Kraken/Platform/Time.h"
#include "Kraken/IO/Window.h"

namespace Kraken {
    struct AppCommandlineArguments {
        int argc;
        char **argv;
    };
    
    class Application {
    public:
        explicit Application(const ApplicationInfo& applicationInfo);
        ~Application();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        
        void Run();
        void Stop();
        
        [[nodiscard]] const ApplicationInfo& GetApplicationInfo() const { return m_ApplicationInfo; }
        [[nodiscard]] Window& GetWindow() const { return *m_Window; }
        static Application& GetInstance() { return *s_Instance; }
    private:
        bool m_ShouldClose = false;
        bool m_Minimized = false;

        std::queue<Event*> m_EventsQueue;
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(const WindowResizeEvent& e);

        Timer m_FullAppTimer;
        FPSTimer m_FPSTimer;
        TimeInstant m_LastFrameTime;

        ApplicationInfo m_ApplicationInfo;
        static Application* s_Instance;
        Scope<Window> m_Window;
        Layerstack m_Layerstack;
    };

    Application* CreateApplication(AppCommandlineArguments cmdlineArguments); // Will be defined by the program
} // Kraken