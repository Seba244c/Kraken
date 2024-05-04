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

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::None:
                return GL_FLOAT;
            case ShaderDataType::Float:
                return GL_FLOAT;
            case ShaderDataType::Float2:
                return GL_FLOAT;
            case ShaderDataType::Float3:
                return GL_FLOAT;
            case ShaderDataType::Float4:
                return GL_FLOAT;
            case ShaderDataType::Mat3:
                return GL_FLOAT;
            case ShaderDataType::Mat4:
                return GL_FLOAT;
            case ShaderDataType::Int:
                return GL_INT;
            case ShaderDataType::Int2:
                return GL_INT;
            case ShaderDataType::Int3:
                return GL_INT;
            case ShaderDataType::Int4:
                return GL_INT;
            case ShaderDataType::Bool:
                return GL_BOOL;
        }

        return GL_FLOAT;
    }
    
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

        constexpr float vertices[3*3+4*3] = {
            -0.5f, -0.5f, 0.0f,  0.8f, 0.2f, 0.8f, 1.0f,
            0.5f,  -0.5f, 0.0f,  0.2f, 0.3f, 0.8f, 1.0f,
            0.0f,   0.5f, 0.0f,  0.8f, 0.8f, 0.2f, 1.0f,
        };

        m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

        {
            const BufferLayout layout = {
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color"}
            };
            m_VertexBuffer->SetLayout(layout);
        }

        uint32_t i = 0;
        const auto& layout = m_VertexBuffer->GetLayout();
        for(const auto& element : layout) {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                reinterpret_cast<const void *>(element.Offset));
            i++;
        }

        constexpr uint32_t indicies[3] = { 0, 1, 2 };
        m_IndexBuffer = IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t));

        const std::string vertexSrc = R"(
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 v_Color;

void main ()
{
    v_Color = a_Color;
    gl_Position = vec4(a_Position, 1.0);
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