//
// Created by sebsn on 05-05-2024.
//

#pragma once
#include <krpch.h>

#include "Kraken/Graphics/VertexArray.h"

namespace Kraken {
    class OpenGLVertexArray final : public VertexArray {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;

        void Bind() const override;
        void Unbind() const override;

        void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) override;
        void SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) override;

        [[nodiscard]] const std::vector<Ref<VertexBuffer>> & GetVertexBuffers() const override { return m_VertexBuffers; }
        [[nodiscard]] const Ref<IndexBuffer> & GetIndexBuffer() const override { return m_IndexBuffer; }
    private:
        uint32_t m_RendererID;
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;
    };
} // Kraken