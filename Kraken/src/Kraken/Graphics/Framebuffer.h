//
// Created by sebsn on 09-05-2024.
//

#pragma once

#include <krpch.h>

namespace Kraken {
    enum class FramebufferTextureFormat {
		None = 0,

		// Color
		RGBA8,
		
		// Depth/stencil
		DEPTH24STENCIL8,
		
		// Defaults
		Depth = DEPTH24STENCIL8
    };

    struct FramebufferTextureSpecification {
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

    struct FramebufferAttachmentSpecification {
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

    struct FramebufferSpecification {
        uint32_t Width = 0, Height = 0;
		FramebufferAttachmentSpecification Attachments;
        uint32_t Samples = 1;
    };

    class Framebuffer {
    public:
        virtual ~Framebuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		[[nodiscard]] virtual const FramebufferSpecification& GetSpecification() const = 0;
    };
} // Kraken
