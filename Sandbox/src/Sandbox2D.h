#pragma once
#include "Kraken.h"

class Sandbox2D final : public Kraken::Layer {
public:
	Sandbox2D();
	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(const Kraken::Timestep ts) override;
	void OnEvent(Kraken::Event& event) override;
private:
    Kraken::OrthographicCameraController m_Camera;
    Kraken::Ref<Kraken::Texture2D> m_Texture;
    Kraken::Ref<Kraken::Font> m_Font;
};