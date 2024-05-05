//
// Created by sebsn on 02-05-2024.
//

#include <glad/gl.h>

#include "OpenGLRendererAPI.h"
#include "OpenGLBuffer.h"
#include "OpenGLShader.h"

namespace Kraken {
    void OpenGLRendererAPI::SetClearColor(const Color color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    Ref<VertexArray> OpenGLRendererAPI::CreateVertexArray() {
        return CreateRef<OpenGLVertexArray>();
    }

    Ref<VertexBuffer> OpenGLRendererAPI::CreateVertexBuffer(const float *vertices, uint32_t size) {
        return CreateRef<OpenGLVertexBuffer>(vertices, size);
    }

    Ref<IndexBuffer> OpenGLRendererAPI::CreateIndexBuffer(const uint32_t *indices, uint32_t count) {
        return CreateRef<OpenGLIndexBuffer>(indices, count);
    }

    Ref<Shader> OpenGLRendererAPI::CreateShader(const std::string &vertexSrc, const std::string &fragmentSrc) {
        return CreateRef<OpenGLShader>(vertexSrc, fragmentSrc);
    }
} // Kraken