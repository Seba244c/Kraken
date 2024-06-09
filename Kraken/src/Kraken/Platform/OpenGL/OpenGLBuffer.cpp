//
// Created by sebsn on 02-05-2024.
//

#include "OpenGLBuffer.h"

namespace Kraken {

    // VERTEX BUFFER
    
    OpenGLVertexBuffer::OpenGLVertexBuffer(const float *vertices, const uint32_t size) {
        KR_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(const uint32_t size) {
        KR_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        KR_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLVertexBuffer::SetData(const void* data, const uint32_t size) {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    void OpenGLVertexBuffer::Bind() const {
        KR_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLVertexBuffer::Unbind() const {
        KR_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    // INDEX BUFFER
    
    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t *indices, const uint32_t count) : m_Count(count) {
        KR_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        KR_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const {
        KR_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLIndexBuffer::Unbind() const {
        KR_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
