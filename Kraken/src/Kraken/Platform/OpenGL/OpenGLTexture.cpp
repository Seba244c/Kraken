#include "OpenGLTexture.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace Kraken {
	namespace Utils {
		static GLenum ImageFormatToGLDataFormat(const ImageFormat format) {
			switch (format) {
				case ImageFormat::RGB8:    return GL_RGB;
				case ImageFormat::RGBA8:   return GL_RGBA;
				case ImageFormat::R8:      return GL_R;
				case ImageFormat::RGBA32F: return GL_RGBA;
			}
			
			KRC_ASSERT(false, "Unkown ImageFormat");
			return 0;
		}
		
		static GLenum ImageFormatToGLInternalFormat(const ImageFormat format) {
			switch (format) {
				case ImageFormat::RGB8:    return GL_RGB8;
				case ImageFormat::RGBA8:   return GL_RGBA8;
				case ImageFormat::R8:	   return GL_R8;
				case ImageFormat::RGBA32F: return GL_RGBA32F;
			}

			KRC_ASSERT(false, "Unkown ImageFormat");
			return 0;
		}

		static GLint FilteringMethodToGLFilter(const FilteringMethod method) {
			switch (method) {
			case FilteringMethod::LINEAR:
				return GL_LINEAR;
			case FilteringMethod::NEAREST:
				return GL_NEAREST;
			case FilteringMethod::MIPMAP_CLOSEST_LINEAR:
				return GL_LINEAR_MIPMAP_NEAREST;
			case FilteringMethod::MIPMAP_CLOSEST_NEAREST:
				return GL_NEAREST_MIPMAP_NEAREST;
			case FilteringMethod::MIPMAP_INTERPOLATED_LINEAR:
				return GL_LINEAR_MIPMAP_LINEAR;
			case FilteringMethod::MIPMAP_INTERPOLATED_NEAREST:
				return GL_NEAREST_MIPMAP_LINEAR;
			}

			KRC_ASSERT(false, "Unkown FilteringMethod");
			return 0;
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(AssetSpecification& assetSpecification) {
        KR_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		char* buffer;
		const auto length = assetSpecification.ToBuf(&buffer);
		
		KR_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(AssetSpecification& assetSpecification)");
		stbi_uc* data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(buffer), length, &width, &height, &channels, 0);
		KRC_ASSERT(data, "Failed to load image!");

		if(data) {
			// Make specification
			m_Width = width;
			m_Height = height;

			ImageFormat format;
			if (channels == 4) {
				format = ImageFormat::RGBA8;
			} else if (channels == 3) {
				format = ImageFormat::RGB8;
			} else {
				KRC_ASSERT(false, "Unkown image format")
				return;
			}
			constexpr auto minify = FilteringMethod::MIPMAP_INTERPOLATED_LINEAR;
			constexpr auto magnify = FilteringMethod::NEAREST;

			m_Specification = {m_Width, m_Height, format, minify, magnify};

			// Create texture
			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, Utils::ImageFormatToGLInternalFormat(format), m_Width, m_Height);

			// Parameters
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Utils::FilteringMethodToGLFilter(minify));
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Utils::FilteringMethodToGLFilter(magnify));
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// Place data
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, Utils::ImageFormatToGLDataFormat(format), GL_UNSIGNED_BYTE, data);
			
			stbi_image_free(data);
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& textureSpecification) {
        KR_PROFILE_FUNCTION();

		m_Specification = textureSpecification;
		m_Width = m_Specification.Width;
		m_Height = m_Specification.Height;
		
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, Utils::ImageFormatToGLInternalFormat(textureSpecification.Format), m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Utils::FilteringMethodToGLFilter(textureSpecification.minifyFilter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Utils::FilteringMethodToGLFilter(textureSpecification.magnifyFilter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
        KR_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(const uint32_t slot) const {
        KR_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size) {
		uint32_t bpp = m_Specification.Format == ImageFormat::RGBA8 ? 4 : 3;
		KRC_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, Utils::ImageFormatToGLDataFormat(m_Specification.Format), GL_UNSIGNED_BYTE, data);
	}
}
