#include "Sprite.h"

namespace Kraken {
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec4& rect) : m_Texture(texture) {
		const float w = static_cast<float>(texture->GetWidth());
		const float h = static_cast<float>(texture->GetHeight());
		
		m_TexCoords[0] = { rect.x / w,				rect.y / h };
		m_TexCoords[1] = { (rect.x + rect.z) / w,	rect.y / h };
		m_TexCoords[2] = { (rect.x + rect.z) / w,	(rect.y + rect.w) / h };
		m_TexCoords[3] = { rect.x / w,				(rect.y + rect.w) / h };
	}

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max) : m_Texture(texture) {
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromGrid(const Ref<Texture2D>& texture, const glm::vec2& cellSize,
		const glm::vec2 spriteCoords, const glm::vec2& spriteSize) {
		const float w = static_cast<float>(texture->GetWidth());
		const float h = static_cast<float>(texture->GetHeight());

		glm::vec2 min = {(spriteCoords.x * cellSize.x) / w, (spriteCoords.y * cellSize.y) / h};
		glm::vec2 max = { ((spriteCoords.x + spriteSize.x) * cellSize.x) / w, ((spriteCoords.y + spriteSize.y) * cellSize.y) / h};

		return CreateRef<SubTexture2D>(texture, min, max);
	}

	SpriteSheet::SpriteSheet(AssetSpecification& assetSpecs) {
		// Data
		Ref<Texture2D> texture;
		glm::vec4 offset;

		// Parse
		for(std::string& line : assetSpecs.ToLines()) {
			std::string value = line.substr(1);
			if(line[0] == 't') texture = AssetsManager::GetTexture2D(Identifier::ParseIdentifier(value));
			else if(line[0] == 'o') {
			    std::istringstream is(value);
			    std::string num;

				std::getline(is, num, ',');
				const float x = std::stoi(num);
				std::getline(is, num, ',');
				const float y = std::stoi(num);
				std::getline(is, num, ',');
				const float w = std::stoi(num);
				std::getline(is, num, ',');
				const float h = std::stoi(num);
				
				offset = { x, texture->GetHeight()-y-h,w,h};
			} else if(line[0] == 's') {
				PushSprite(value[0], texture, offset);
			}
		}
	}

	Ref<SubTexture2D> SpriteSheet::GetSprite(const char id) {
		return m_SpriteMap[id];
	}

	void SpriteSheet::PushSprite(const char c, Ref<Texture2D>& texture, glm::vec4& offset) {
		m_SpriteMap[c] = CreateRef<SubTexture2D>(texture, offset);
	}
}
