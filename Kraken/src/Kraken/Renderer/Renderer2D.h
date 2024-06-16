#pragma once

#include <krpch.h>

#include "Sprite.h"
#include "Kraken/Graphics/Camera.h"
#include "Kraken/Graphics/Texture.h"
#undef DrawText

namespace Kraken {
	class Font;

	class Renderer2D {
	public:
		struct Quad {
			const glm::vec2 Position = {0.0f, 0.0f};
			const glm::vec2 Size = {1.0f, 1.0f};
			const float Rotation = 0.0f;
			const Ref<Texture2D> Texture;
			const float TilingFactor = 1.0f;
			const Color Color = Colors::White;
		};

		struct Sprite {
			const glm::vec2 Position = {0.0f, 0.0f};
			const glm::vec2 Size = {1.0f, 1.0f};
			const float Rotation = 0.0f;
			const Ref<SubTexture2D> Texture;
			const Color Color = Colors::White;
		};

		struct Text {
			const std::string String;
			const Ref<Font> Font;
			const glm::vec2 Position = {0.0f, 0.0f};
			const Color Color = Colors::White;
			const float Kerning = 0.0f;
			const float LineSpacing = 0.0f;
		};
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void EndScene();
		static void Flush();
		
		// Primitives
		static void DrawQuad(const Quad &q);
		static void DrawRotatedQuad(const Quad &q);
		static void DrawSprite(const Sprite &s);
		static void DrawRotatedSprite(const Sprite &s);
		static void DrawQuad(const glm::mat4& transform, const Color& color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec2 textureCoords[], float tilingFactor, const Color& color);
		static void DrawText(const Text &t);
		static void DrawText(const std::string &string, const Ref<Font> &font, const glm::mat4 &transform, const Color &color = Colors::White, float kerning = 0.0f, float lineSpacing = 0.0f);

		static void DrawTileMap(const glm::vec2& pos, TileMap &tileMap);

		static void StartBatch();
		static void NextBatch();
	};
}
