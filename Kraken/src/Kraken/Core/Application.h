//
// Created by sebsn on 21-04-2024.
//

#ifndef KR_APPLICATION_H
#define KR_APPLICATION_H
#include "Kraken/Events/ApplicationEvents.h"
#include "Kraken/Events/KeyEvents.h"
#include "Kraken/IO/GLFW.h"

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

        void Run();

        [[nodiscard]] const ApplicationInfo& GetApplicationInfo() const { return m_ApplicationInfo; }
        static Application& GetInstance() { return *s_Instance; }
    private:
        bool m_ShouldClose = false;

        std::queue<Event*> m_EventsQueue;
        bool OnKey(KeyPressedEvent& e);
        bool OnWindowClose(WindowCloseEvent& e);
        
        ApplicationInfo m_ApplicationInfo;
        static Application* s_Instance;
        Window* m_Window;
    };

    Application* CreateApplication(AppCommandlineArguments cmdlineArguments); // Will be defined by the program
} // Kraken

#endif //KR_APPLICATION_H
