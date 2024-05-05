//
// Created by sebsn on 02-05-2024.
//

#ifndef KR_OPENGLRENDERERAPI_H
#define KR_OPENGLRENDERERAPI_H

#include "Kraken/Graphics/RendererAPI.h"
#include "Kraken/Platform/OpenGL/OpenGLVertexArray.h"

namespace Kraken {
    class OpenGLRendererAPI : public RendererAPI {
    public:
        void SetClearColor(Color color) override;

        Ref<VertexArray> CreateVertexArray() override;

        Ref<VertexBuffer> CreateVertexBuffer(const float *vertices, uint32_t size) override;

        Ref<IndexBuffer> CreateIndexBuffer(const uint32_t *indices, uint32_t count) override;

        Ref<Shader> CreateShader(const std::string &vertexSrc, const std::string &fragmentSrc) override;
    };
} // Kraken

#endif //KR_OPENGLRENDERERAPI_H
