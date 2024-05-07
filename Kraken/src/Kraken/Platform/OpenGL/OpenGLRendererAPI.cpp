//
// Created by sebsn on 02-05-2024.
//

#include <glad/gl.h>

#include "OpenGLRendererAPI.h"
#include "OpenGLBuffer.h"
#include "OpenGLShader.h"

namespace Kraken {
    void OpenGLRendererAPI::Init() {
		// Culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Enable depth test
        glEnable(GL_DEPTH_TEST);

        // Enable transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void OpenGLRendererAPI::SetClearColor(const Color color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount) {
        vertexArray->Bind();
        const uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
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