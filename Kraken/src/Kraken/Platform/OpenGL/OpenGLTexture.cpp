#include "OpenGLTexture.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace Kraken {
	OpenGLTexture2D::OpenGLTexture2D(AssetSpecification& assetSpecification) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(assetSpecification.GetPath().string().c_str(), &width, &height, &channels, 0);
		KRC_ASSERT(data, "Failed to load image!");

		if(data) {
			m_Width = width;
			m_Height = height;

			GLenum storageFormat = 0, imageFormat = 0;
			if (channels == 4) {
				storageFormat = GL_RGBA8;
				imageFormat = GL_RGBA;
			} else if (channels == 3) {
				storageFormat = GL_RGB8;
				imageFormat = GL_RGB;
			}

			// Create texture
			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, storageFormat, m_Width, m_Height);

			// Parameters
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// Place data
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, imageFormat, GL_UNSIGNED_BYTE, data);
			
			stbi_image_free(data);
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(const uint32_t slot) const {
		glBindTextureUnit(slot, m_RendererID);
	}
}
