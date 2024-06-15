//
// Created by sebsn on 02-05-2024.
//

#include "OpenGLGraphicsAPI.h"
#include "OpenGLBuffer.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLShader.h"
#include "OpenGLUniformBuffer.h"
#include "OpenGLTexture.h"

namespace Kraken {
    void OpenGLGraphicsAPI::Init() {
        KR_PROFILE_FUNCTION();

		// Culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Enable depth test
        glEnable(GL_DEPTH_TEST);

        // Enable transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void OpenGLGraphicsAPI::SetClearColor(const Color color) {
        KR_PROFILE_FUNCTION();

        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLGraphicsAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLGraphicsAPI::DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount) {
        KR_PROFILE_FUNCTION();

        vertexArray->Bind();
        const uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLGraphicsAPI::SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) {
        glViewport(x, y, width, height);
    }

    Ref<VertexArray> OpenGLGraphicsAPI::CreateVertexArray() {
        return CreateRef<OpenGLVertexArray>();
    }

    Ref<VertexBuffer> OpenGLGraphicsAPI::CreateVertexBuffer(const float *vertices, uint32_t size) {
        return CreateRef<OpenGLVertexBuffer>(vertices, size);
    }

    Ref<VertexBuffer> OpenGLGraphicsAPI::CreateVertexBuffer(uint32_t size) {
        return CreateRef<OpenGLVertexBuffer>(size);
    }

    Ref<IndexBuffer> OpenGLGraphicsAPI::CreateIndexBuffer(const uint32_t *indices, uint32_t count) {
        return CreateRef<OpenGLIndexBuffer>(indices, count);
    }

    Ref<Shader> OpenGLGraphicsAPI::CreateShader(const std::string &vertexSrc, const std::string &fragmentSrc) {
        return CreateRef<OpenGLShader>(vertexSrc, fragmentSrc);
    }

    Ref<Shader> OpenGLGraphicsAPI::CreateShader(AssetSpecification& specs) {
        KRC_ASSERT(specs.GetIdentifier().name.ends_with(".glsl"), "File is not a glsl shader file!")
        return CreateRef<OpenGLShader>(specs);
    }

    Ref<Framebuffer> OpenGLGraphicsAPI::CreateFramebuffer(const FramebufferSpecification &spec) {
        return CreateRef<OpenGlFramebuffer>(spec);
    }

    Ref<UniformBuffer> OpenGLGraphicsAPI::CreateUniformBuffer(uint32_t size, uint32_t binding) {
        return CreateRef<OpenGLUniformBuffer>(size, binding);
    }

    Ref<Texture2D> OpenGLGraphicsAPI::CreateTexture2D(AssetSpecification& specs) {
        return CreateRef<OpenGLTexture2D>(specs);
    }

    Ref<Texture2D> OpenGLGraphicsAPI::CreateTexture2D(TextureSpecification& specs) {
        return CreateRef<OpenGLTexture2D>(specs);
    }
} // Kraken