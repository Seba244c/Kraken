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

	class Texture {
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture {
	public:
		
	};
}