//
// Created by sebsn on 21-04-2024.
//

#include "Kraken.h"

class SandboxLayer final : public Kraken::Layer {
public:
    SandboxLayer() : Layer("Sandbox") {
        // Temp rendering
        Kraken::RenderCommand::SetClearColor(Kraken::Colors::DarkGray);
        constexpr float vertices[4*3+4*4] = {
            -1.0f, -1.0f, 0.0f,   0.8f, 0.2f, 0.8f, 1.0f,
            1.0f,  -1.0f, 0.0f,   0.2f, 0.3f, 0.8f, 1.0f,
            1.0f,   1.0f, 0.0f,   0.8f, 0.8f, 0.2f, 1.0f,
            -1.0,   1.0f, 0.0f,   0.2f, 0.8f, 0.2f, 1.0f
        };

        const auto vb = Kraken::RenderCommand::CreateVertexBuffer(vertices, sizeof(vertices));
        vb->SetLayout({
            { Kraken::ShaderDataType::Float3, "a_Position" },
            { Kraken::ShaderDataType::Float4, "a_Color"}
        });


        constexpr uint32_t indicies[6] = { 0, 1, 2, 2, 3, 0 };
        const auto ib = Kraken::RenderCommand::CreateIndexBuffer(indicies, sizeof(indicies) / sizeof(uint32_t));

        m_VertexArray = Kraken::RenderCommand::CreateVertexArray();
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

        m_Shader = Kraken::RenderCommand::CreateShader(vertexSrc, fragmentSrc);
        m_Camera = Kraken::CreateScope<Kraken::OrthographicCamera>(-2.0f, 2.0f, -2.0f, 2.0f);
    };

    void OnUpdate() override {
        // movement
        auto direction = glm::vec3(0.0f);

        if(Kraken::Input::IsKeyPressed(Kraken::Key::W)) direction.y += 1;
        if(Kraken::Input::IsKeyPressed(Kraken::Key::S)) direction.y -= 1;
        if(Kraken::Input::IsKeyPressed(Kraken::Key::D)) direction.x += 1;
        if(Kraken::Input::IsKeyPressed(Kraken::Key::A)) direction.x -= 1;
        m_Camera->SetPosition(m_Camera->GetPosition() + direction * 0.01f);


        {
            Kraken::RenderCommand::Clear();
            Kraken::Renderer::BeginScene(*m_Camera);
            Kraken::Renderer::Submit(m_Shader, m_VertexArray);
            Kraken::Renderer::EndScene();
        }
    }

    void OnEvent(Kraken::Event &event) override {
        
    }
private:
    Kraken::Ref<Kraken::VertexArray> m_VertexArray;
    Kraken::Ref<Kraken::Shader> m_Shader;
    Kraken::Scope<Kraken::OrthographicCamera> m_Camera;
};

class Sandbox : public Kraken::Application {
public:
    explicit Sandbox(const Kraken::ApplicationInfo &applicationInfo)
        : Application(applicationInfo) {
        PushLayer(new SandboxLayer());
    }
};

Kraken::Application *Kraken::CreateApplication(AppCommandlineArguments cmdlineArguments) {
    ApplicationInfo info;
    info.Name = "Kraken Sandbox";
    info.Author = "ssnoer";
    info.Version = "1.0.0";
    
    return new Sandbox(info);
}
