//
// Created by sebsn on 02-05-2024.
//

#ifndef KR_OPENGLBUFFER_H
#define KR_OPENGLBUFFER_H

#include "Kraken/Graphics/Buffer.h"

namespace Kraken {
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(const float* vertices, uint32_t size);
        ~OpenGLVertexBuffer() override;

        void Bind() const override;
        void Unbind() const override;
    private:
        uint32_t m_RendererID;
    };

    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        [[nodiscard]] virtual uint32_t GetCount() const { return m_Count; }
    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };
}

#endif //KR_OPENGLBUFFER_H
