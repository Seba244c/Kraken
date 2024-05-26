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
            { Kraken::ShaderDataType::Float4, "a_Color" },
        });


        constexpr uint32_t indicies[6] = { 0, 1, 2, 2, 3, 0 };
        const auto ib = Kraken::RenderCommand::CreateIndexBuffer(indicies, sizeof(indicies) / sizeof(uint32_t));

        m_VertexArray = Kraken::RenderCommand::CreateVertexArray();
        m_VertexArray->AddVertexBuffer(vb);
        m_VertexArray->SetIndexBuffer(ib);

        const std::string vertexSrc = R"(
            #version 450 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;
            layout(std140, binding = 0) uniform Camera
            {
                mat4 u_mViewProjection;
            };

            layout(location = 0) out vec4 v_Color;

            void main ()
            {
                v_Color = a_Color;
                gl_Position = u_mViewProjection * vec4(a_Position, 1.0);
            }

        )";

        const std::string fragmentSrc = R"(
            #version 450 core


            layout(location = 0) in vec4 v_Color;
            layout(location = 0) out vec4 o_Color;

            void main ()
            {
                o_Color = v_Color;
            }

        )";

        m_Shader = Kraken::RenderCommand::CreateShader(vertexSrc, fragmentSrc);
        m_Camera = Kraken::CreateScope<Kraken::OrthographicCamera>(-2.0f, 2.0f, -2.0f, 2.0f);
        Kraken::Renderer::Init();
        Kraken::AssetsManager::RegisterAssetProvider("Sandbox", Kraken::CreateScope<Kraken::FolderAssetProvider>("assets/"));
        auto a = Kraken::AssetsManager::Get({ "Sandbox", "shaders/Sandbox.glsl" });
        KRC_TRACE("Sandbox:shaders/Sandbox.glsl = {0}", a.GetPath().string());
    };

    void OnUpdate(const Kraken::Timestep ts) override {
        // movement
        auto direction = glm::vec3(0.0f);
        float scaledSpeed = 6.0f * ts;

        if(Kraken::Input::IsKeyPressed(Kraken::Key::W)) direction.y += 1;
        if(Kraken::Input::IsKeyPressed(Kraken::Key::S)) direction.y -= 1;
        if(Kraken::Input::IsKeyPressed(Kraken::Key::D)) direction.x += 1;
        if(Kraken::Input::IsKeyPressed(Kraken::Key::A)) direction.x -= 1;
        if(length(direction) > 0)
            m_Camera->SetPosition(m_Camera->GetPosition() + normalize(direction) * scaledSpeed);

        Kraken::RenderCommand::Clear();
        Kraken::Renderer::BeginScene(*m_Camera);
        Kraken::Renderer::SetShader(m_Shader);

        Kraken::Renderer::Submit(m_VertexArray);
        Kraken::Renderer::EndScene();
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
