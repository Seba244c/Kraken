//
// Created by sebsn on 21-04-2024.
//

#include "Application.h"

#include "Kraken/Debug/ImGuiLayer.h"
#include "Kraken/Events/KeyEvents.h"

#include "Kraken/Graphics/RenderCommand.h"
#include "Kraken/Graphics/Renderer.h"
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
        RenderCommand::SetClearColor(Colors::DarkGray);
        constexpr float vertices[4*3+4*4] = {
            -1.0f, -1.0f, 0.0f,   0.8f, 0.2f, 0.8f, 1.0f,
            1.0f,  -1.0f, 0.0f,   0.2f, 0.3f, 0.8f, 1.0f,
            1.0f,   1.0f, 0.0f,   0.8f, 0.8f, 0.2f, 1.0f,
            -1.0,   1.0f, 0.0f,   0.2f, 0.8f, 0.2f, 1.0f
        };

        auto vb = RenderCommand::CreateVertexBuffer(vertices, sizeof(vertices));
        vb->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color"}
        });


        constexpr uint32_t indicies[6] = { 0, 1, 2, 2, 3, 0 };
        auto ib = RenderCommand::CreateIndexBuffer(indicies, sizeof(indicies) / sizeof(uint32_t));

        m_VertexArray = RenderCommand::CreateVertexArray();
        m_VertexArray->AddVertexBuffer(vb);
        m_VertexArray->SetIndexBuffer(ib);

        const std::string vertexSrc = R"(
#version 450 core

uniform mat4 u_mProjectionView;

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 v_Color;

void main ()
{
    v_Color = a_Color;
    gl_Position = u_mProjectionView * vec4(a_Position, 1.0);
}

)";

        const std::string fragmentSrc = R"(
#version 450 core

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;

void main ()
{
    o_Color = v_Color;
}

)";
        
        m_Shader = RenderCommand::CreateShader(vertexSrc, fragmentSrc);
        m_Camera = CreateScope<OrthographicCamera>(-2.0f, 2.0f, -2.0f, 2.0f);
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

                dispatcher.Dispatch<WindowCloseEvent>(KR_BIND_EVENT_FN(Application::OnWindowClose));
                dispatcher.Dispatch<WindowResizeEvent>(KR_BIND_EVENT_FN(Application::OnWindowResize));
                Input::Event(*e); // For all input events

                for (auto it = m_Layerstack.end(); it != m_Layerstack.begin();) {
                    if(e->Handled) break;
                    (*--it)->OnEvent(*e);
                }

                delete e;
            }

            // movement
            auto direction = glm::vec3(0.0f);

            if(Input::IsKeyPressed(Key::W)) direction.y += 1;
            if(Input::IsKeyPressed(Key::S)) direction.y -= 1;
            if(Input::IsKeyPressed(Key::D)) direction.x += 1;
            if(Input::IsKeyPressed(Key::A)) direction.x -= 1;
            m_Camera->SetPosition(m_Camera->GetPosition() + direction * 0.01f);

            // Update layers
            {
                RenderCommand::Clear();
                Renderer::BeginScene(*m_Camera);
                Renderer::Submit(m_Shader, m_VertexArray);
                Renderer::EndScene();
            }

            for(Layer* layer : m_Layerstack)
                layer->OnUpdate();

            // Swap buffers
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