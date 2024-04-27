//
// Created by sebsn on 21-04-2024.
//

#include "Application.h"

#include "Kraken/Events/KeyEvents.h"

namespace Kraken {
	Application* Application::s_Instance = nullptr;
    
    Application::Application(const ApplicationInfo &applicationInfo) : m_ApplicationInfo(applicationInfo) {
        KRC_ASSERT(s_Instance == nullptr, "Instance shouldn't already exist");
        s_Instance = this;

        KRC_INFO("Appstate: Create Window");
        m_Window = new Window({.initializeFullscreen = false, .initializeHidden = true});
        m_Window->SetEventCallback(KR_BIND_EVENT_FN(Application::OnEvent));
    }

    void Application::OnEvent(Event &e) {
		EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(KR_BIND_EVENT_FN(Application::OnKey));
    }

    bool Application::OnKey(KeyPressedEvent& e) {
        if(e.KeyCode() == Key::ESCAPE) KRC_INFO("Pressed escape");
        return false;
    }
    
    void Application::Run() {
        m_Window->Show();
        
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