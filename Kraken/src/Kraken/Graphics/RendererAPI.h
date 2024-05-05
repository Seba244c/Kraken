//
// Created by sebsn on 29-04-2024.
//

#ifndef KR_RENDERAPI_H
#define KR_RENDERAPI_H

#include "Kraken/Graphics/VertexArray.h"
#include "Kraken/Graphics/Shader.h"

namespace Kraken {
    class RendererAPI {
    public:
        enum class API {
            None = 0, OpenGL = 1
        };
    public:
        virtual ~RendererAPI() = default;
        static API GetAPI() { return s_API; }
        static Scope<RendererAPI> Create();

        virtual void SetClearColor(Color color) = 0;

        virtual Ref<VertexArray> CreateVertexArray() = 0;
        virtual Ref<VertexBuffer> CreateVertexBuffer(const float* vertices, uint32_t size) = 0;
		virtual Ref<IndexBuffer> CreateIndexBuffer(const uint32_t *indices, uint32_t count) = 0;
        virtual Ref<Shader> CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) = 0;
    private:
        static API s_API;
    };
}

#endif //KR_RENDERAPI_H
