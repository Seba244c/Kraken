#include "Sandbox2D.h"

float s_Rotation = 0;

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_Camera(800.0f/600.0f) {
	Kraken::AssetsManager::RegisterAssetProvider("Sandbox", Kraken::CreateScope<Kraken::FolderAssetProvider>("Sandbox", "assets/"));
    m_Texture = Kraken::AssetsManager::GetTexture2D({"Sandbox", "textures/Zote.jpg"});
	
	Kraken::Renderer::Init();
	Kraken::RenderCommand::SetClearColor(Kraken::Colors::DarkGray);
}

void Sandbox2D::OnAttach() {
}

void Sandbox2D::OnDetach() {
	
}

void Sandbox2D::OnUpdate(const Kraken::Timestep ts) {
    KR_PROFILE_FUNCTION();

	// Update
    m_Camera.OnUpdate(ts);

    // Rendering
    Kraken::RenderCommand::Clear();
    Kraken::Renderer2D::BeginScene(m_Camera.GetCamera());
    Kraken::Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, Kraken::Colors::Red);
    Kraken::Renderer2D::DrawQuad({1.0f, 0.0f}, {1.0f, 1.0f}, Kraken::Colors::Blue);
    s_Rotation += ts * 75;
    Kraken::Renderer2D::DrawRotatedQuad({2.0f, 0.0f}, s_Rotation, {0.5f, 0.5f}, Kraken::Colors::Green);
    Kraken::Renderer2D::DrawRotatedQuad({0.0f, 2.0f}, -s_Rotation, {1.0f, 1.0f}, m_Texture, 3);
    
    Kraken::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(Kraken::Event& event) {
	m_Camera.OnEvent(event);
}
