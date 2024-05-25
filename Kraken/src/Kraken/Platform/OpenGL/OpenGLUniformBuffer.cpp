//
// Created by sebsn on 25-05-2024.
//

#include "OpenGLUniformBuffer.h"

namespace Kraken {
    OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding) {
        glCreateBuffers(1, &m_RendererID);
        glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW); // TODO: investigate usage hint
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer() {
		glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLUniformBuffer::SetData(const void *data, uint32_t size, uint32_t offset) {
		glNamedBufferSubData(m_RendererID, offset, size, data);
    }
} // Kraken