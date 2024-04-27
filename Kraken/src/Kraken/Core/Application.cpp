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
        m_Window->SetEventCallback([this](Event* e){ m_EventsQueue.push(e); });
    }

    void Application::Run() {
        m_Window->Show();
        
        KRC_INFO("Appstate: Main loop");
        while(!m_ShouldClose) {
            m_Window->PollEvents();

            // Handle events
            while(!m_EventsQueue.empty()) {
                EventDispatcher dispatcher(m_EventsQueue.front());
                m_EventsQueue.pop();
                
                dispatcher.Dispatch<KeyPressedEvent>(KR_BIND_EVENT_FN(Application::OnKey));
                dispatcher.Dispatch<WindowCloseEvent>(KR_BIND_EVENT_FN(Application::OnWindowClose));
            }
        }
    }
    
    bool Application::OnKey(KeyPressedEvent& e) {
        if(e.KeyCode() == Key::ESCAPE) m_ShouldClose = true;
        return true;
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        m_ShouldClose = true;
        return true;
    }

    Application::~Application() {
        KRC_INFO("Appstate: Cleanup");
        delete m_Window;
        GLFW::Terminate();
    }
} // Kraken