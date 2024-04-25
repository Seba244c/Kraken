//
// Created by sebsn on 21-04-2024.
//

#include "Application.h"

namespace Kraken {
	Application* Application::s_Instance = nullptr;
    
    Application::Application(const ApplicationInfo &applicationInfo) : m_ApplicationInfo(applicationInfo) {
        KRC_ASSERT(s_Instance == nullptr, "Instance shouldn't already exist");
        s_Instance = this;

        KRC_INFO("Appstate: Create Window");
        m_Window = new Window({.title = "Sandbox"});
    }

    void Application::Run() {
        KRC_INFO("Appstate: Main loop");
        while(!m_Window->ShouldClose()) {
            m_Window->PollEvents();
        }
    }

    Application::~Application() {
        KRC_INFO("Appstate: Cleanup");
        delete m_Window;
        GLFW::Terminate();
    }
} // Kraken