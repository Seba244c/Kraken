//
// Created by sebsn on 02-05-2024.
//

#pragma once
#include <krpch.h>

#include "Kraken/Graphics/Buffer.h"

namespace Kraken {
    class OpenGLVertexBuffer final : public VertexBuffer {
    public:
        OpenGLVertexBuffer(const float* vertices, uint32_t size);
        ~OpenGLVertexBuffer() override;

        [[nodiscard]] const BufferLayout & GetLayout() const override { return m_Layout; }
        void SetLayout(const BufferLayout &layout) override { m_Layout = layout; }

        void Bind() const override;
        void Unbind() const override;
    private:
        uint32_t m_RendererID;
		BufferLayout m_Layout;
    };

    class OpenGLIndexBuffer final : public IndexBuffer {
    public:
        OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        [[nodiscard]] uint32_t GetCount() const override { return m_Count; }
    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };
}
