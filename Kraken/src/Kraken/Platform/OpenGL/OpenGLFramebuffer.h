//
// Created by sebsn on 09-05-2024.
//

#pragma once
#include "Kraken/Graphics/Framebuffer.h"

namespace Kraken {
    class OpenGlFramebuffer final : public Framebuffer {
    public:
        OpenGlFramebuffer(const FramebufferSpecification& spec);
        ~OpenGlFramebuffer() override;
	    void Invalidate();

        void Bind() override;
        void Unbind() override;

        void Resize(uint32_t width, uint32_t height) override;
        const FramebufferSpecification& GetSpecification() const override { return m_Specification; };
    private:
		uint32_t m_RendererID = 0;
        FramebufferSpecification m_Specification;

        // Invalidate() sorts the texturespecification into theese to catagories
		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

        // Textures
		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
    };
} // Kraken
