#pragma once
#include "krpch.h"

namespace Kraken {
	enum class ImageFormat {
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	enum class FilteringMethod {
		LINEAR = 0,
		NEAREST,

		MIPMAP_CLOSEST_LINEAR,        //  takes the nearest mipmap level and samples that level using linear interpolation. (GL_LINEAR_MIPMAP_NEAREST)

		MIPMAP_CLOSEST_NEAREST,       //  takes the nearest mipmap to match the pixel size and (GL_NEAREST_MIPMAP_NEAREST)

		MIPMAP_INTERPOLATED_LINEAR,   //  linearly interpolates between the two closest mipmaps and samples the interpolated level via linear interpolation. (GL_LINEAR_MIPMAP_LINEAR)

		MIPMAP_INTERPOLATED_NEAREST,  //  linearly interpolates between the two mipmaps that most closely match the size of a pixel and samples the interpolated level via nearest neighbor interpolation. (GL_NEAREST_MIPMAP_LINEAR)
	};

	struct TextureSpecification {
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		FilteringMethod minifyFilter = FilteringMethod::MIPMAP_INTERPOLATED_LINEAR;
		FilteringMethod magnifyFilter = FilteringMethod::MIPMAP_INTERPOLATED_NEAREST;
	};

	class Texture {
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	protected:
		TextureSpecification m_Specification;
	};

	class Texture2D : public Texture {
	public:
		
	};
}