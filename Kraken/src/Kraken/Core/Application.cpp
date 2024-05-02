//
// Created by sebsn on 21-04-2024.
//

#include "Application.h"

#include "Kraken/Debug/ImGuiLayer.h"
#include "Kraken/Events/KeyEvents.h"

#include "glad/gl.h"
#include "Kraken/IO/Input.h"

namespace Kraken {
	Application* Application::s_Instance = nullptr;
    
    Application::Application(const ApplicationInfo &applicationInfo) : m_ApplicationInfo(applicationInfo) {
        KRC_ASSERT(s_Instance == nullptr, "Instance shouldn't already exist");
        s_Instance = this;

        KRC_INFO("Appstate: Create Window");
        
        m_Window = Window::Create(WindowSpecs({.initializeFullscreen = false, .initializeHidden = true}));
        m_Window->SetEventCallback([this](Event* e){ m_EventsQueue.push(e); }); // Here the applications takes ownership of the event
        
        // Add Debug overlay
        PushOverlay(new ImGuiLayer());

        // Temp rendering
        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        constexpr float vertices[3*3] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        constexpr uint32_t indicies[3] = { 0, 1, 2 };
        m_IndexBuffer = IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t));

        const std::string vertexSrc = R"(
#version 450 core

layout(location = 0) in vec3 a_Position;

out vec3 v_Position;

void main ()
{
    v_Position = a_Position;
    gl_Position = vec4(a_Position, 1.0);
}

)";

        const std::string fragmentSrc = R"(
#version 450 core

layout(location = 0) out vec4 o_Color;

in vec3 v_Position;

void main ()
{
    o_Color = vec4(v_Position * 0.5 + 0.5, 1.0);
}

)";
        
        m_Shader = Shader::Create(vertexSrc, fragmentSrc);
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

                Input::Event(*e);

                delete e;
            }

            // Update layers
            glClearColor(Colors::DarkGray.r, Colors::DarkGray.g, Colors::DarkGray.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr); // Index buffer is already attached to the vertex array
            
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