#include "Renderer2D.h"

#include "glm/ext/matrix_transform.hpp"
#include "Kraken/Graphics/RenderCommand.h"
#include "Kraken/Graphics/UniformBuffer.h"

namespace Kraken {
	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		uint32_t TexIndex;
	};

    struct Renderer2DData {
		static constexpr uint32_t MaxQuads = 20000;
		static constexpr uint32_t MaxVertices = MaxQuads * 4;
		static constexpr uint32_t MaxIndices = MaxQuads * 6;
		static constexpr uint32_t MaxTextureSlots = 32;

		// Camera
        struct CameraData {
            glm::mat4 ViewProjection;
        };
        CameraData CameraBuffer;
        Ref<UniformBuffer> CameraUniformBuffer;

		// Shaders
	    Ref<Shader> QuadShader;
		
		// Quads
	    Ref<VertexArray> QuadVertexArray;
		uint32_t QuadIndexCount = 0;
		Ref<VertexBuffer> QuadVertexBuffer;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
		glm::vec4 QuadVertexPositions[4];
		
		Ref<Texture2D> WhiteTexture;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture
    };
	
    static Renderer2DData s_Data;

	void Renderer2D::Init() { // Creates necessary renderer resources
		s_Data.CameraUniformBuffer = RenderCommand::CreateUniformBuffer(sizeof(Renderer2DData::CameraData), 0);

		// Texture
		s_Data.WhiteTexture = RenderCommand::CreateTexture(TextureSpecification{});
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		uint32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		/// Primitives
		/// Quad
		s_Data.QuadVertexArray = RenderCommand::CreateVertexArray();

	    s_Data.QuadVertexBuffer = RenderCommand::CreateVertexBuffer(s_Data.MaxVertices * sizeof(QuadVertex));
	    s_Data.QuadVertexBuffer->SetLayout({
	        { ShaderDataType::Float3, "a_Position" },
	        { ShaderDataType::Float4, "a_Color" },
	        { ShaderDataType::Float2, "a_TexCoord" },
	        { ShaderDataType::UInt, "a_TexIndex" },
	    });
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		// Calculate quad indices for s_Data.MaxIndices
		auto quadIndices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		
		Ref<IndexBuffer> quadIB = RenderCommand::CreateIndexBuffer(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		// Shaders
	    s_Data.QuadShader = RenderCommand::CreateShader(R"(
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in uint a_TexIndex;
layout(std140, binding = 0) uniform Camera
{
    mat4 u_mViewProjection;
};

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) out VertexOutput Output;
layout (location = 2) out flat uint v_TexIndex;

void main ()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
    gl_Position = u_mViewProjection * vec4(a_Position, 1.0);
}
			)", R"(
#version 450 core

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout(location = 0) in VertexOutput Input;
layout(location = 2) in flat uint v_TexIndex;

layout(location = 0) out vec4 o_Color;

layout(binding = 0) uniform sampler2D u_Textures[32];

void main ()
{
	vec4 texColor = Input.Color;
	texColor *= texture(u_Textures[v_TexIndex], Input.TexCoord);

	if (texColor.a == 0.0)
		discard;

	o_Color = texColor;
}
			)");

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown() {
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera) {
        s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene() {
		Flush();
	}

	void Renderer2D::Flush() {
		if (s_Data.QuadIndexCount) {
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
			s_Data.QuadShader->Bind();

			// Bind textures
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);

			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		}
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Color& color) {
		DrawQuad({ position.x, position.y, 1.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Color& color) {
		DrawQuad(
			translate(glm::mat4(1.0f), position) * scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})
			, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture,
		const Color& color) {
		DrawQuad({ position.x, position.y, 1.0f }, size, texture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture,
		const Color& color) {
		DrawQuad(
			translate(glm::mat4(1.0f), position) * scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})
			, texture, color);     
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const float rotationZ, const glm::vec2& size,
	                                 const Color& color) {
		DrawRotatedQuad({ position.x, position.y, 1.0f }, rotationZ, size, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const float rotationZ, const glm::vec2& size,
		const Color& color) {
		DrawQuad(
			translate(glm::mat4(1.0f), position)
			* rotate(glm::mat4(1.0f), glm::radians(rotationZ), {0.0f, 0.0f, 1.0f}) *
			scale(glm::mat4(1.0f), {size.x, size.y, 1.0f })
			, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const float rotationZ, const glm::vec2& size,
		const Ref<Texture2D> texture, const Color& color) {
		DrawRotatedQuad({ position.x, position.y, 1.0f }, rotationZ, size, texture, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const float rotationZ, const glm::vec2& size,
		const Ref<Texture2D> texture, const Color& color) {
		DrawQuad(
			translate(glm::mat4(1.0f), position)
			* rotate(glm::mat4(1.0f), glm::radians(rotationZ), {0.0f, 0.0f, 1.0f}) *
			scale(glm::mat4(1.0f), {size.x, size.y, 1.0f})
			, texture, color);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Color& color) {
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = 0; // White texture
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const Color& color) {
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		// Get texture index
		uint32_t texIndex = 0;
		for (int i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i] == *texture) {
				texIndex = i;
				break;
			}
		}

		if (texIndex == 0) { // i.e. no matching texture
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			texIndex = s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::StartBatch() {
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::NextBatch() {
		Flush();
		StartBatch();
	}
}
