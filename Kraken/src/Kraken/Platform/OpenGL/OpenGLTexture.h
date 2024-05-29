#pragma once
#include "krpch.h"
#include "Kraken/Assets/Asset.h"
#include "Kraken/Graphics/Texture.h"

namespace Kraken {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(AssetSpecification& assetSpecification);
		~OpenGLTexture2D() override;
		
		uint32_t GetWidth() const override { return m_Width;  }
		uint32_t GetHeight() const override { return m_Height; }
		void Bind(uint32_t slot) const override;
	private:
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
	};
}