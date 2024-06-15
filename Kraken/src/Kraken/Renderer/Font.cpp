#include "Font.h"

#include "Kraken/Graphics/RenderCommand.h"

namespace Kraken {
	template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
	static Ref<Texture2D> CreateAtlas(const std::string& fontName, float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
			const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height) {
		KR_PROFILE_FUNCTION();

		msdf_atlas::GeneratorAttributes attributes;
		attributes.config.overlapSupport = true;
		attributes.scanlinePass = true;

		msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
		generator.setAttributes(attributes);
		generator.setThreadCount(8);
		generator.generate(glyphs.data(), static_cast<int>(glyphs.size()));

		msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>) generator.atlasStorage();

		TextureSpecification spec;
		spec.Width = bitmap.width;
		spec.Height = bitmap.height;
		spec.Format = ImageFormat::RGB8;
		spec.magnifyFilter = FilteringMethod::LINEAR;
		spec.minifyFilter = FilteringMethod::LINEAR;

		Ref<Texture2D> texture = RenderCommand::CreateTexture(spec);
		texture->SetData((void *)bitmap.pixels, bitmap.width * bitmap.height * 3);
		return texture;
	}

	Font::Font(AssetSpecification& assetSpecs) : m_FontData(new MSDFFontData()) {
		KR_PROFILE_FUNCTION();

		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();

		// Load font library data
		char* buffer;
		const auto length = assetSpecs.ToBuf(&buffer);
		msdfgen::FontHandle* font = loadFontData(ft, reinterpret_cast<const msdfgen::byte*>(buffer), length);

		if(!font) {
			KRC_ERROR("Failed to load font!");
			return;
		}

		// Create charset
		msdf_atlas::Charset charset;
		for (CharsetRange range : s_DefaultCharsetRanges) {
			for (uint32_t c = range.Begin; c <= range.End; c++)
				charset.add(c);
		}

		// Build font geometry
		double fontScale = 1.0;
		m_FontData->FontGeometry = msdf_atlas::FontGeometry(&m_FontData->Glyphs);
		int glyphsLoaded = m_FontData->FontGeometry.loadCharset(font, fontScale, charset);
		KRC_TRACE("Loaded {} glyphs from font (out of {})", glyphsLoaded, charset.size());

		// Atlaspacker
		double emSize = 40.0;

		msdf_atlas::TightAtlasPacker atlasPacker;
		atlasPacker.setPixelRange(2.0);
		atlasPacker.setMiterLimit(1.0);
		atlasPacker.setPadding(0);
		atlasPacker.setScale(emSize);
		int remaining = atlasPacker.pack(m_FontData->Glyphs.data(), static_cast<int>(m_FontData->Glyphs.size()));
		KRC_ASSERT(remaining == 0)

		int width, height;
		atlasPacker.getDimensions(width, height);
		emSize = atlasPacker.getScale();

		// Generate atlas
		uint64_t coloringSeed = 0;
		constexpr unsigned long long lcgMultiplier = 6364136223846793005ull;
		constexpr double angleThreshold = 3.0;

		unsigned long long glyphSeed = coloringSeed;
		for (msdf_atlas::GlyphGeometry& glyph : m_FontData->Glyphs) {
			glyphSeed *= lcgMultiplier;
			glyph.edgeColoring(msdfgen::edgeColoringInkTrap, angleThreshold, glyphSeed);
		}

		KRC_TRACE("Generating Atlas: Width={0}, Height={1}, Scale={2}", width, height, emSize);
		const auto texture = CreateAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>("Test", static_cast<float>(emSize), m_FontData->Glyphs, m_FontData->FontGeometry, width, height);
		m_AtlasTexture = texture;
	}

	Font::~Font() {
		delete m_FontData;
	}
}
