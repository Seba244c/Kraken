//
// Created by sebsn on 21-04-2024.
//

#ifndef KR_APPLICATION_H
#define KR_APPLICATION_H
#include "Layerstack.h"
#include "Kraken/Events/ApplicationEvents.h"
#include "Kraken/Events/KeyEvents.h"
#include "Kraken/IO/Window.h"
#include "Kraken/Graphics/Shader.h"
#include "Kraken/Graphics/Buffer.h"
#include "Kraken/Graphics/VertexArray.h"

namespace Kraken {
    struct ApplicationInfo {
        std::string Name;
        std::string Author;
        std::string Version;
    };
    
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

        std::queue<Event*> m_EventsQueue;
        bool OnKey(KeyPressedEvent& e);
        bool OnWindowClose(WindowCloseEvent& e);
        
        ApplicationInfo m_ApplicationInfo;
        static Application* s_Instance;
        Scope<Window> m_Window;
        Layerstack m_Layerstack;

        Ref<VertexArray> m_VertexArray;
        Ref<Shader> m_Shader;
    };

    Application* CreateApplication(AppCommandlineArguments cmdlineArguments); // Will be defined by the program
} // Kraken

#endif //KR_APPLICATION_H
