#pragma once

#include <krpch.h>
#include "Kraken/Graphics/Camera.h"
#include "Kraken/Graphics/Texture.h"

namespace Kraken {
	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void EndScene();
		static void Flush();
		
		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Color& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Color& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const Color& color = Colors::White);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const Color& color = Colors::White);

		static void DrawRotatedQuad(const glm::vec2& position, const float rotationZ, const glm::vec2& size, const Color& color);
		static void DrawRotatedQuad(const glm::vec3& position, const float rotationZ, const glm::vec2& size, const Color& color);
		static void DrawRotatedQuad(const glm::vec2& position, const float rotationZ, const glm::vec2& size, const Ref<Texture2D> texture, const Color& color = Colors::White);
		static void DrawRotatedQuad(const glm::vec3& position, const float rotationZ, const glm::vec2& size, const Ref<Texture2D> texture, const Color& color = Colors::White);

		static void DrawQuad(const glm::mat4& transform, const Color& color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const Color& color);
	private:
		static void StartBatch();
		static void NextBatch();
	};
}
