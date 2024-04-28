//
// Created by sebsn on 21-04-2024.
//

#include "Application.h"

#include "Kraken/Debug/ImGuiLayer.h"
#include "Kraken/Events/KeyEvents.h"

#include "glad/gl.h"

namespace Kraken {
	Application* Application::s_Instance = nullptr;
    
    Application::Application(const ApplicationInfo &applicationInfo) : m_ApplicationInfo(applicationInfo) {
        KRC_ASSERT(s_Instance == nullptr, "Instance shouldn't already exist");
        s_Instance = this;

        KRC_INFO("Appstate: Create Window");
        m_Window = CreateScope<Window>(WindowSpecs({.initializeFullscreen = false, .initializeHidden = true}));
        m_Window->SetEventCallback([this](Event* e){ m_EventsQueue.push(e); });
        

        // Add Debug overlay
        PushOverlay(new ImGuiLayer());
    }

    void Application::Run() {
        m_Window->Show();
        
        KRC_INFO("Appstate: Main loop");
        while(!m_ShouldClose) {
            m_Window->PollEvents();

            // Handle events
            while(!m_EventsQueue.empty()) {
                const auto e = m_EventsQueue.front();
                EventDispatcher dispatcher(e);
                m_EventsQueue.pop();
                
                dispatcher.Dispatch<KeyPressedEvent>(KR_BIND_EVENT_FN(Application::OnKey));
                dispatcher.Dispatch<WindowCloseEvent>(KR_BIND_EVENT_FN(Application::OnWindowClose));

                for (auto it = m_Layerstack.end(); it != m_Layerstack.begin();) {
                    if(e->Handled) break;
                    (*--it)->OnEvent(*e);
                }
            }

            // Update layers
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            for(Layer* layer : m_Layerstack)
                layer->OnUpdate();

            // Swap buffers
            m_Window->SwapBuffers();
        }
    }

    void Application::Stop() {
        m_ShouldClose = true;
    }

    bool Application::OnKey(KeyPressedEvent& e) {
        if(e.KeyCode() == Key::ESCAPE) Stop();
        return true;
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        Stop();
        return true;
    }

    Application::~Application() {
        KRC_INFO("Appstate: Cleanup");
    }

    void Application::PushLayer(Layer* layer) {
        m_Layerstack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer) {
        m_Layerstack.PushOverlay(layer);
        layer->OnAttach();
    }
} // Kraken