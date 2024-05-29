//
// Created by sebsn on 21-04-2024.
//

#include "Kraken.h"

class SandboxLayer final : public Kraken::Layer {
public:
    SandboxLayer() : Layer("Sandbox") {
        Kraken::AssetsManager::RegisterAssetProvider("Sandbox", Kraken::CreateScope<Kraken::FolderAssetProvider>("Sandbox", "assets/"));

        // Temp rendering
        Kraken::RenderCommand::SetClearColor(Kraken::Colors::DarkGray);
        constexpr float vertices[4*3+2*4] = {
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
            1.0f,  -1.0f, 0.0f,   1.0f, 0.0f,
            1.0f,   1.0f, 0.0f,   1.0f, 1.0f,
            -1.0,   1.0f, 0.0f,   0.0f, 1.0f
        };

        const auto vb = Kraken::RenderCommand::CreateVertexBuffer(vertices, sizeof(vertices));
        vb->SetLayout({
            { Kraken::ShaderDataType::Float3, "a_Position" },
            { Kraken::ShaderDataType::Float2, "a_TexCoord" },
        });

        constexpr uint32_t indicies[6] = { 0, 1, 2, 2, 3, 0 };
        const auto ib = Kraken::RenderCommand::CreateIndexBuffer(indicies, sizeof(indicies) / sizeof(uint32_t));

        m_VertexArray = Kraken::RenderCommand::CreateVertexArray();
        m_VertexArray->AddVertexBuffer(vb);
        m_VertexArray->SetIndexBuffer(ib);

        m_Shader = Kraken::AssetsManager::GetShader({ "Sandbox", "shaders/Sandbox.glsl" });
        m_Camera = Kraken::CreateScope<Kraken::OrthographicCamera>(-2.0f, 2.0f, -2.0f, 2.0f);

        Kraken::Renderer::Init();
    }

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

    	// Render Triangle
        Kraken::Renderer::SetShader(m_Shader);
        Kraken::Renderer::Submit(m_VertexArray);

        // Render Texture
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
