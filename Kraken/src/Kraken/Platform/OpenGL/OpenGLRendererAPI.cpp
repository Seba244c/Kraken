//
// Created by sebsn on 02-05-2024.
//

#include "OpenGLRendererAPI.h"
#include "OpenGLBuffer.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLShader.h"
#include "OpenGLUniformBuffer.h"

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

    void OpenGLRendererAPI::SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) {
        glViewport(x, y, width, height);
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

    Ref<Shader> OpenGLRendererAPI::CreateShader(AssetSpecification& specs) {
        KRC_ASSERT(specs.GetPath().extension() == ".glsl", "File is not a glsl shader file!")
        return CreateRef<OpenGLShader>(specs);
    }

    Ref<Framebuffer> OpenGLRendererAPI::CreateFramebuffer(const FramebufferSpecification &spec) {
        return CreateRef<OpenGLFramebuffer>(spec);
    }

    Ref<UniformBuffer> OpenGLRendererAPI::CreateUniformBuffer(uint32_t size, uint32_t binding) {
        return CreateRef<OpenGLUniformBuffer>(size, binding);
    }
} // Kraken