//
// Created by sebsn on 09-05-2024.
//

#include "OpenGLFramebuffer.h"

namespace Kraken {
	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace Utils {
        static GLenum FramebufferTextureFormatToGL(FramebufferTextureFormat format) {
	        switch (format) {
	           case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
	        }

            KRC_ASSERT(false)
            return 0;
        }

        static bool IsDepthFormat(FramebufferTextureFormat format) {
	        switch(format) {
	        case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
	        default:                                        return false;
	        }
        }

        static GLenum TextureTarget(bool multisampled) {
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

        static void BindTexture(bool multisampled, uint32_t id) {
			glBindTexture(TextureTarget(multisampled), id);
		}

        static void CreateTextures(bool multisampled, uint32_t* outID, size_t count) {
			glCreateTextures(TextureTarget(multisampled), count, outID);
        }

        static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum storageFormat, uint32_t width, uint32_t height, int index) {
	        const bool multisampled = samples > 1;
			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			} else {
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, storageFormat, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
        }

        static void AttachDepthTexture(uint32_t id, int samples, GLenum internalFormat, GLenum attachmentType, uint32_t width, uint32_t height) {
	        const bool multisampled = samples > 1;
			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			} else {
				glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
        }
    }

    OpenGlFramebuffer::OpenGlFramebuffer(const FramebufferSpecification &specs) : m_Specification(specs) {
        KR_PROFILE_FUNCTION();

        for(auto spec : m_Specification.Attachments.Attachments) {
	        if(!Utils::IsDepthFormat(spec.TextureFormat)) m_DepthAttachmentSpecification = spec;
            else                            m_ColorAttachmentSpecifications.emplace_back(spec);
        }

		Invalidate();
    }

    void OpenGlFramebuffer::Invalidate() {
        if(m_RendererID) {
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);
        }

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        bool multisample = m_Specification.Samples > 1;

        // Create Color Attachments
        if (!m_ColorAttachmentSpecifications.empty()) {
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size()); // Creates all the textures

            // Bind all textures
            for(size_t i = 0; i < m_ColorAttachments.size(); i++) {
	            Utils::BindTexture(multisample, m_ColorAttachments[i]);

                switch(m_ColorAttachmentSpecifications[i].TextureFormat) {
                case FramebufferTextureFormat::RGBA8:
                    Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
                    break;
                default:
                    KRC_ASSERT(false, "Unkown ColorAttachment format")
                    break;
                }
            }
        }

        // Create depth
        if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None) {
			Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
			Utils::BindTexture(multisample, m_DepthAttachment);

            switch (m_DepthAttachmentSpecification.TextureFormat) {
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
            default:
            	KRC_ASSERT(false, "Unkown DepthAttachment format")
			}
        }

        if (m_ColorAttachments.size() > 1) {
			KRC_ASSERT(m_ColorAttachments.size() <= 4);
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);

		} else if (m_ColorAttachments.empty()) {
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		KRC_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    OpenGlFramebuffer::~OpenGlFramebuffer() {
        KR_PROFILE_FUNCTION();

		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGlFramebuffer::Bind() {
        KR_PROFILE_FUNCTION();

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }

    void OpenGlFramebuffer::Unbind() {
        KR_PROFILE_FUNCTION();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGlFramebuffer::Resize(uint32_t width, uint32_t height) {
        KR_PROFILE_FUNCTION();

		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize) {
			KRC_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
			return;
		}

		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
    }
} // Kraken
