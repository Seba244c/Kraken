#include "Renderer2D.h"

#include "glm/ext/matrix_transform.hpp"
#include "Kraken/Renderer/Font.h"
#include "Kraken/Graphics/RenderCommand.h"
#include "Kraken/Graphics/UniformBuffer.h"

namespace Kraken {
    struct QuadVertex {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        uint32_t TexIndex;
        float TilingFactor;
    };

    struct TextVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
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
        Ref<Shader> TextShader;
        
        // Quads
        Ref<VertexArray> QuadVertexArray;
        uint32_t QuadIndexCount = 0;
        Ref<VertexBuffer> QuadVertexBuffer;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;
        glm::vec4 QuadVertexPositions[4];
        //

        // Text
        Ref<VertexArray> TextVertexArray;
        uint32_t TextIndexCount = 0;
        Ref<VertexBuffer> TextVertexBuffer;
        TextVertex* TextVertexBufferBase = nullptr;
        TextVertex* TextVertexBufferPtr = nullptr;
		Ref<Texture2D> FontAtlasTexture;
        // 
        
        Ref<Texture2D> WhiteTexture;
        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 = white texture
    };
    
    static Renderer2DData s_Data;

    void Renderer2D::Init() { // Creates necessary renderer resources
        KR_PROFILE_FUNCTION();

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
            { ShaderDataType::Float, "a_TilingFactor" },
        });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
        /// Text
        s_Data.TextVertexArray = RenderCommand::CreateVertexArray();

        s_Data.TextVertexBuffer = RenderCommand::CreateVertexBuffer(s_Data.MaxVertices * sizeof(TextVertex));
        s_Data.TextVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
        });
        s_Data.TextVertexArray->AddVertexBuffer(s_Data.TextVertexBuffer);
        s_Data.TextVertexBufferBase = new TextVertex[s_Data.MaxVertices];

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
layout(location = 4) in float a_TilingFactor;
layout(std140, binding = 0) uniform Camera
{
    mat4 u_mViewProjection;
};

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TilingFactor;
};

layout (location = 0) out VertexOutput Output;
layout (location = 3) out flat uint v_TexIndex;

void main ()
{
    Output.Color = a_Color;
    Output.TexCoord = a_TexCoord;
    Output.TilingFactor = a_TilingFactor;
    v_TexIndex = a_TexIndex;
    gl_Position = u_mViewProjection * vec4(a_Position, 1.0);
}
            )", R"(
#version 450 core

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TilingFactor;
};

layout(location = 0) in VertexOutput Input;
layout(location = 3) in flat uint v_TexIndex;

layout(location = 0) out vec4 o_Color;

layout(binding = 0) uniform sampler2D u_Textures[32];

void main ()
{
    vec4 texColor = Input.Color;
    texColor *= texture(u_Textures[v_TexIndex], Input.TexCoord * Input.TilingFactor);

    if (texColor.a == 0.0)
        discard;

    o_Color = texColor;
}
            )");
    	s_Data.TextShader = RenderCommand::CreateShader(R"(
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
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

void main ()
{
    Output.Color = a_Color;
    Output.TexCoord = a_TexCoord;
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
layout(location = 0) out vec4 o_Color;

layout(binding = 0) uniform sampler2D u_FontAtlas;

float screenPxRange() {
	const float pxRange = 2.0; // set to distance field's pixel range
    vec2 unitRange = vec2(pxRange)/vec2(textureSize(u_FontAtlas, 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(Input.TexCoord);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main ()
{
    vec4 texColor = Input.Color * texture(u_FontAtlas, Input.TexCoord);

	vec3 msd = texture(u_FontAtlas, Input.TexCoord).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	if (opacity == 0.0)
		discard;

	vec4 bgColor = vec4(0.0);
    o_Color = mix(bgColor, Input.Color, opacity);
	if (o_Color.a == 0.0)
		discard;
}
            )");

        s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    }

    void Renderer2D::Shutdown() {
        KR_PROFILE_FUNCTION();

        delete[] s_Data.QuadVertexBufferBase;
        delete[] s_Data.TextVertexBufferBase;
    }

    void Renderer2D::BeginScene(const Camera& camera) {
        KR_PROFILE_FUNCTION();

        s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
        s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

        StartBatch();
    }

    void Renderer2D::EndScene() {
        KR_PROFILE_FUNCTION();

        Flush();
    }

    void Renderer2D::Flush() {
        if (s_Data.QuadIndexCount) {
            KR_PROFILE_SCOPE("Quads");
            uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
            s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
            s_Data.QuadShader->Bind();

            // Bind textures
            for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
                s_Data.TextureSlots[i]->Bind(i);

            RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
        }

        if(s_Data.TextIndexCount) {
            KR_PROFILE_SCOPE("Text");
            uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.TextVertexBufferPtr - (uint8_t*)s_Data.TextVertexBufferBase);
            s_Data.TextVertexBuffer->SetData(s_Data.TextVertexBufferBase, dataSize);
            s_Data.TextShader->Bind();

            s_Data.FontAtlasTexture->Bind();

            RenderCommand::DrawIndexed(s_Data.TextVertexArray, s_Data.TextIndexCount);
	        
        }
    }

    void Renderer2D::DrawQuad(const Quad &q) {
        KR_PROFILE_FUNCTION();
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        // TODO: The texture check is also expensive.
        if(q.Texture)
            DrawQuad(
                translate(glm::mat4(1.0f), {q.Position.x, q.Position.y, 0.0f})
                * scale(glm::mat4(1.0f), {q.Size.x, q.Size.y, 1.0f})
            , q.Texture, textureCoords, q.TilingFactor, q.Color);
        else 
            DrawQuad(
                translate(glm::mat4(1.0f), {q.Position.x, q.Position.y, 0.0f})
                * scale(glm::mat4(1.0f), {q.Size.x, q.Size.y, 1.0f})
                , q.Color);
    }

    void Renderer2D::DrawRotatedQuad(const Quad &q) {
        KR_PROFILE_FUNCTION();
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        // TODO: The texture check
        if(q.Texture)
            DrawQuad(
                translate(glm::mat4(1.0f), {q.Position.x, q.Position.y, 0.0f})
                * rotate(glm::mat4(1.0f), glm::radians(q.Rotation), {0.0f, 0.0f, 1.0f}) *
                scale(glm::mat4(1.0f), {q.Size.x, q.Size.y, 1.0f})
            , q.Texture, textureCoords, q.TilingFactor, q.Color);
        else 
            DrawQuad(
                translate(glm::mat4(1.0f), {q.Position.x, q.Position.y, 0.0f})
                * rotate(glm::mat4(1.0f), glm::radians(q.Rotation), {0.0f, 0.0f, 1.0f}) *
                scale(glm::mat4(1.0f), {q.Size.x, q.Size.y, 1.0f})
                , q.Color);
    }

    void Renderer2D::DrawSprite(const Sprite& s) {
            DrawQuad(
                translate(glm::mat4(1.0f), {s.Position.x, s.Position.y, 0.0f})
                * scale(glm::mat4(1.0f), {s.Size.x, s.Size.y, 1.0f})
            , s.Texture->GetTexture(), s.Texture->GetTexCoords(), 1, s.Color);
    }

    void Renderer2D::DrawRotatedSprite(const Sprite& s) {
            DrawQuad(
                translate(glm::mat4(1.0f), {s.Position.x, s.Position.y, 0.0f})
                * rotate(glm::mat4(1.0f), glm::radians(s.Rotation), {0.0f, 0.0f, 1.0f}) *
                scale(glm::mat4(1.0f), {s.Size.x, s.Size.y, 1.0f})
            , s.Texture->GetTexture(), s.Texture->GetTexCoords(), 1, s.Color);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Color& color) {
        KR_PROFILE_FUNCTION();

        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        constexpr float tilingFactor = 1.0f;
        constexpr int textureIndex = 0;

        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            NextBatch();

        for (size_t i = 0; i < quadVertexCount; i++) {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex; // White texture
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec2 textureCoords[], const float tilingFactor, const Color& color) {
        KR_PROFILE_FUNCTION();

        constexpr size_t quadVertexCount = 4;

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
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
    }

    void Renderer2D::DrawText(const std::string& string, Ref<Font> font, const glm::mat4& transform) {
        // TEMP
    	glm::vec4 Color{ 1.0f };
		float Kerning = 0.0f;
		float LineSpacing = 0.0f;
        //

		const auto& fontGeometry = font->GetFontData()->FontGeometry;
		const auto& metrics = fontGeometry.getMetrics();
		Ref<Texture2D> fontAtlas = font->GetAtlasTexture();

        // If another font is already being used to render
        if(s_Data.FontAtlasTexture != fontAtlas && s_Data.TextIndexCount) Flush();
        s_Data.FontAtlasTexture = fontAtlas;
        
		double x = 0.0;
		double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
		double y = 0.0;
		const float spaceGlyphAdvance = fontGeometry.getGlyph(' ')->getAdvance();

		for (size_t i = 0; i < string.size(); i++) {
			char character = string[i];

            switch(character) { // Handle spacing characters
            case '\r': continue;
            case '\n':
				x = 0;
				y -= fsScale * metrics.lineHeight + LineSpacing;
				continue;
            case '\t':
				x += 4.0f * (fsScale * spaceGlyphAdvance + Kerning);
				continue;
            case ' ':
				float advance = spaceGlyphAdvance;
                if (i < string.size() - 1) {
	                char nextCharacter = string[i + 1];
					double dAdvance;
					fontGeometry.getAdvance(dAdvance, character, nextCharacter);
					advance = static_cast<float>(dAdvance);
                }

                x += fsScale * advance + Kerning;
                continue;
            }

            // Get ? glyph if glyph is not found
            auto glyph = fontGeometry.getGlyph(character);
			if (!glyph)
				glyph = fontGeometry.getGlyph('?');
			if (!glyph) {
				KRC_ERROR("Failed to find glyph for characters {0} and {1}", character, '?');
				return;
			}

            // Get texture coordinates
            double al, ab, ar, at;
			glyph->getQuadAtlasBounds(al, ab, ar, at);
			glm::vec2 texCoordMin((float)al, (float)ab);
			glm::vec2 texCoordMax((float)ar, (float)at);

			float texelWidth = 1.0f / fontAtlas->GetWidth();
			float texelHeight = 1.0f / fontAtlas->GetHeight();
			texCoordMin *= glm::vec2(texelWidth, texelHeight);
			texCoordMax *= glm::vec2(texelWidth, texelHeight);

            // Get position coordinates
			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);
			glm::vec2 quadMin((float)pl, (float)pb);
			glm::vec2 quadMax((float)pr, (float)pt);

            // Times scale and move positions to baseline
            quadMin *= fsScale, quadMax *= fsScale;
			quadMin += glm::vec2(x, y);
			quadMax += glm::vec2(x, y);

            // Add to queue
            s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = Color;
			s_Data.TextVertexBufferPtr->TexCoord = texCoordMin;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = Color;
			s_Data.TextVertexBufferPtr->TexCoord = { texCoordMin.x, texCoordMax.y };
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = Color;
			s_Data.TextVertexBufferPtr->TexCoord = texCoordMax;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = Color;
			s_Data.TextVertexBufferPtr->TexCoord = { texCoordMax.x, texCoordMin.y };
			s_Data.TextVertexBufferPtr++;
			s_Data.TextIndexCount += 6;

            if (i < string.size() - 1) { // Advance by character width
				double advance = glyph->getAdvance();
				char nextCharacter = string[i + 1];
				fontGeometry.getAdvance(advance, character, nextCharacter);

				x += fsScale * advance + Kerning;
			}
		}
    }

    void Renderer2D::DrawTileMap(const glm::vec2& pos, TileMap& tileMap) {
        const auto spriteSheet = tileMap.GetSpriteSheet();
        const auto grid = tileMap.GetTiles();
        const auto size = tileMap.GetSize();

        for(uint32_t y = 0; y < size; y++) {
            for(uint32_t x = 0; x < size; x++) {
                DrawSprite({.Position = {(x - size / 2.0f) + pos.x, (size - y - size / 2.0f) + pos.y}, .Texture = spriteSheet->GetSprite(grid[x + y * size])});
            }
        }
    }

    void Renderer2D::StartBatch() {
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextIndexCount = 0;
		s_Data.TextVertexBufferPtr = s_Data.TextVertexBufferBase;

        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::NextBatch() {
        Flush();
        StartBatch();
    }
}
