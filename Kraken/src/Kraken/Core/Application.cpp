//
// Created by sebsn on 21-04-2024.
//

#include "Application.h"

#include "Kraken/Events/KeyEvents.h"

#include "Kraken/Graphics/Renderer.h"
#include "Kraken/IO/Input.h"

#include "Kraken/Platform/PlatformUtils.h"

namespace Kraken {
	Application* Application::s_Instance = nullptr;
    
    Application::Application(const ApplicationInfo &applicationInfo) : m_LastFrameTime(TimeInstant(0.0f)),
                                                                       m_ApplicationInfo(applicationInfo) {
        KRC_ASSERT(s_Instance == nullptr, "Instance shouldn't already exist");
        s_Instance = this;

        // Platform
        PlatformInit::Init();
        m_FullAppTimer = Timer::Start();

        KRC_INFO("Appstate: Create Window");

        m_Window = Window::Create(WindowSpecs({.initializeFullscreen = false, .initializeHidden = true}));
        m_Window->SetEventCallback([this](Event *e) { m_EventsQueue.push(e); });
        // Here the applications takes ownership of the event
    }

    void Application::Run() {
        m_Window->Show();
        FPSTimer::Start();
        
        KRC_INFO("Appstate: Main loop");
        while(!m_ShouldClose) {
            // Handle events
            m_Window->PollEvents();

            while(!m_EventsQueue.empty()) {
                const auto e = m_EventsQueue.front();
                EventDispatcher dispatcher(e);
                m_EventsQueue.pop();

                dispatcher.Dispatch<WindowCloseEvent>(KR_BIND_EVENT_FN(Application::OnWindowClose));
                dispatcher.Dispatch<WindowResizeEvent>(KR_BIND_EVENT_FN(Application::OnWindowResize));
                Input::Event(*e); // For all input events

                for (auto it = m_Layerstack.end(); it != m_Layerstack.begin();) {
                    if(e->Handled) break;
                    (*--it)->OnEvent(*e);
                }

                delete e;
            }

            // Time
            m_FPSTimer.Frame();
            TimeInstant now = Platform::Time::Now();
            const auto deltaTime = now - m_LastFrameTime;
            m_LastFrameTime = now;

            // Update layers and render
            for(Layer* layer : m_Layerstack)
                layer->OnUpdate(deltaTime);

            m_Window->SwapBuffers();
        }
    }

    void Application::Stop() {
        m_ShouldClose = true;
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        Stop();
        return true;
    }

    bool Application::OnWindowResize(const WindowResizeEvent &e) {
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
        return false;
    }

    Application::~Application() {
        KRC_INFO("Appstate: Cleanup");
        KRC_INFO("App Timer: {0}s", m_FullAppTimer.Get().GetSeconds());
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