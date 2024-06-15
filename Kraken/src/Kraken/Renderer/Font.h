#pragma once
#include <krpch.h>

#include "Kraken/Graphics/Texture.h"
#include "Kraken/Assets/Asset.h"

#include "msdfgen/msdfgen.h"
#include "msdfgen/msdfgen-ext.h"
#undef INFINITE
#include "msdf-atlas-gen/msdf-atlas-gen.h"

namespace Kraken {
	struct MSDFFontData {
		std::vector<msdf_atlas::GlyphGeometry> Glyphs;
		msdf_atlas::FontGeometry FontGeometry;
	};

	// Charset range
	struct CharsetRange {
		uint32_t Begin, End;
	};

	static constexpr CharsetRange s_DefaultCharsetRanges[] = {
		{ 0x0020, 0x00FF } // 32-255
	};

	class Font {
	public:
		Font(AssetSpecification &assetSpecs);
		~Font();

		[[nodiscard]] const MSDFFontData* GetFontData() const { return m_FontData; };
		[[nodiscard]] Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }
	private:
		MSDFFontData *m_FontData;
		Ref<Texture2D> m_AtlasTexture;
	};
}