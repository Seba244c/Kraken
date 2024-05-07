//
// Created by sebsn on 29-04-2024.
//

#pragma once
#include <krpch.h>

#include "RendererAPI.h"

namespace Kraken {
    class Renderer {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene();
        static void EndScene();

        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray/*, const glm::mat4& transform = glm::mat4(1.0f)*/);

        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    };    
}
