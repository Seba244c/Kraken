#pragma once

#include <krpch.h>

#include "Kraken/Assets/Asset.h"
#include "Kraken/Graphics/Texture.h"

namespace Kraken {
	class SubTexture2D {
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec4& rect);
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		[[nodiscard]] Ref<Texture2D> GetTexture() const { return m_Texture; }
		[[nodiscard]] const glm::vec2* GetTexCoords() const { return m_TexCoords; }
		[[nodiscard]] static Ref<SubTexture2D> CreateFromGrid(const Ref<Texture2D>& texture, const glm::vec2& cellSize, const glm::vec2 spriteCoords, const glm::vec2& spriteSize = {1,1});
	private:
		const Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};

	class SpriteSheet {
	public:
		SpriteSheet(AssetSpecification& assetSpecs);
		[[nodiscard]] Ref<SubTexture2D> GetSprite(char id);
	private:
		void PushSprite(char c, Ref<Texture2D>& texture, glm::vec4& offset);
	private:
		std::unordered_map<char, Ref<SubTexture2D>> m_SpriteMap;
	};

	class TileMap {
	public:
		TileMap(uint32_t size, const Ref<SpriteSheet> &spriteSheet, const char* map) : m_Size(size), m_SpriteSheet(spriteSheet), m_Tiles(map) {}
		[[nodiscard]] uint32_t GetSize() const { return m_Size; }
		[[nodiscard]] const Ref<SpriteSheet>& GetSpriteSheet() { return m_SpriteSheet; }
		[[nodiscard]] const char* GetTiles() const { return m_Tiles; }
	private:
		const uint32_t m_Size;
		Ref<SpriteSheet> m_SpriteSheet;
		const char* m_Tiles;
	};
}