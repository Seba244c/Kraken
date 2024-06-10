#pragma once

#include <krpch.h>
#include "Kraken/Graphics/Camera.h"
#include "Kraken/Graphics/Texture.h"

namespace Kraken {
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
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void EndScene();
		static void Flush();
		
		// Primitives
		static void DrawQuad(const Quad &q);
		static void DrawRotatedQuad(const Quad &q);
		static void DrawQuad(const glm::mat4& transform, const Color& color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const Color& color);
	private:
		static void StartBatch();
		static void NextBatch();
	};
}
