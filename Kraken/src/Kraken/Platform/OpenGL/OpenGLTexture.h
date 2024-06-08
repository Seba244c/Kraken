#pragma once
#include <krpch.h>
#include "Kraken/Assets/Asset.h"
#include "Kraken/Graphics/Texture.h"

namespace Kraken {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(AssetSpecification& assetSpecification);
		OpenGLTexture2D(const TextureSpecification& textureSpecification);
		~OpenGLTexture2D() override;
		
		[[nodiscard]] uint32_t GetWidth() const override { return m_Width;  }
		[[nodiscard]] uint32_t GetHeight() const override { return m_Height; }
		[[nodiscard]] uint32_t GetRendererID() const override { return m_RendererID; }
		void Bind(uint32_t slot) const override;
		void SetData(void* data, uint32_t size) override;

		bool operator==(const Texture& other) const override {
			return m_RendererID == other.GetRendererID();
		}
	private:
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
	};
}