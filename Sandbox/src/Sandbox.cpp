//
// Created by sebsn on 21-04-2024.
//

#define KRAKEN_APP
#include "Kraken.h"
#include "Sandbox2D.h"

class SandboxLayer final : public Kraken::Layer {
public:
    SandboxLayer() : Layer("Sandbox"), m_Camera(800.0f/600.0f) {
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
        m_Texture = Kraken::AssetsManager::GetTexture2D({"Sandbox", "textures/Zote.jpg"});

        Kraken::Renderer::Init();
    }

    void OnUpdate(const Kraken::Timestep ts) override {
        // Update
    	m_Camera.OnUpdate(ts);

        // Render
        Kraken::RenderCommand::Clear();
        Kraken::Renderer::BeginScene(m_Camera.GetCamera());

    	// Render Texture
        Kraken::Renderer::SetShader(m_Shader);
        m_Texture->Bind();
        m_Shader->SetInt("u_Texture", 0);
        Kraken::Renderer::Submit(m_VertexArray);

        // Render Texture
        Kraken::Renderer::EndScene();
    }

    void OnEvent(Kraken::Event &event) override {
        m_Camera.OnEvent(event);
    }
private:
    Kraken::Ref<Kraken::VertexArray> m_VertexArray;
    Kraken::Ref<Kraken::Shader> m_Shader;
    Kraken::Ref<Kraken::Texture> m_Texture;
    Kraken::OrthographicCameraController m_Camera;
};

class Sandbox : public Kraken::Application {
public:
    explicit Sandbox(const Kraken::ApplicationInfo &applicationInfo)
        : Application(applicationInfo) {
		Kraken::AssetsManager::RegisterAssetProvider("Sandbox", Kraken::CreateScope<Kraken::FolderAssetProvider>("Sandbox", "assets/"));
		Kraken::Renderer::Init();
		Kraken::RenderCommand::SetClearColor(Kraken::Colors::DarkGray);

        //PushLayer(new SandboxLayer());
        PushLayer(new Sandbox2D());
    }
};

Kraken::Application *Kraken::CreateApplication(AppCommandlineArguments cmdlineArguments) {
    ApplicationInfo info;
    info.Name = "Kraken Sandbox";
    info.Author = "ssnoer";
    info.Version = "1.0.0";
    
    return new Sandbox(info);
}
