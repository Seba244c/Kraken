//
// Created by sebsn on 05-05-2024.
//

#include "OpenGLVertexArray.h"

namespace Kraken {
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::None:
                return GL_FLOAT;
            case ShaderDataType::Float:
                return GL_FLOAT;
            case ShaderDataType::Float2:
                return GL_FLOAT;
            case ShaderDataType::Float3:
                return GL_FLOAT;
            case ShaderDataType::Float4:
                return GL_FLOAT;
            case ShaderDataType::Mat3:
                return GL_FLOAT;
            case ShaderDataType::Mat4:
                return GL_FLOAT;
            case ShaderDataType::UInt:
                return GL_UNSIGNED_INT;
            case ShaderDataType::Int2:
                return GL_INT;
            case ShaderDataType::Int3:
                return GL_INT;
            case ShaderDataType::Int4:
                return GL_INT;
            case ShaderDataType::Bool:
                return GL_BOOL;
        }

        KRC_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray() {
        KR_PROFILE_FUNCTION();

        glCreateVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        KR_PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const {
        KR_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const {
        KR_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) {
        KR_PROFILE_FUNCTION();

        KRC_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex Buffer has no layout!");
        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        uint32_t i = 0;
        const auto& layout = vertexBuffer->GetLayout();
        for(const auto& element : layout) {
            glEnableVertexAttribArray(i);
            if(element.Type == ShaderDataType::UInt) {
	            glVertexAttribIPointer(i,
                    element.GetComponentCount(),
                    GL_UNSIGNED_INT,
                    layout.GetStride(),
                    reinterpret_cast<const void *>(element.Offset));

            } else {
	            glVertexAttribPointer(i,
	                element.GetComponentCount(),
	                ShaderDataTypeToOpenGLBaseType(element.Type),
	                element.Normalized ? GL_TRUE : GL_FALSE,
	                layout.GetStride(),
	                reinterpret_cast<const void *>(element.Offset));
            }
            i++;
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) {
        KR_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
} // Kraken