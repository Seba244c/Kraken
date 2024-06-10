#include "Sandbox2D.h"

static Kraken::TileMap* s_TileMap;
static Kraken::Ref<Kraken::SpriteSheet> s_Sheet;

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_Camera(800.0f/600.0f) {
	Kraken::AssetsManager::RegisterAssetProvider("Sandbox", Kraken::CreateScope<Kraken::FolderAssetProvider>("Sandbox", "assets/"));
	s_Sheet = Kraken::AssetsManager::GetSpriteSheet({"Sandbox", "textures/Overworld.sht"});

	s_TileMap = new Kraken::TileMap(7, s_Sheet,
		"wddwddw"
			"ddddddd"
			"ddddddd"
			"ddddddd"
			"wdddddw"
			"wwdddww"
			"wwwdwww");
	
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
	Kraken::Renderer2D::DrawTileMap({}, *s_TileMap);

    Kraken::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(Kraken::Event& event) {
	m_Camera.OnEvent(event);
}
