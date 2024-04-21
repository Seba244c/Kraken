//
// Created by sebsn on 21-04-2024.
//

#ifndef KR_APPLICATION_H
#define KR_APPLICATION_H

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

        void Run();

        [[nodiscard]] const ApplicationInfo& GetApplicationInfo() const { return m_ApplicationInfo; }
    private:
        ApplicationInfo m_ApplicationInfo;
        Application* s_Instance = nullptr;
    };

    Application* CreateApplication(AppCommandlineArguments cmdlineArguments); // Will be defined by the program
} // Kraken

#endif //KR_APPLICATION_H
