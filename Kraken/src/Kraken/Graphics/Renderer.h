//
// Created by sebsn on 29-04-2024.
//

#pragma once
#include <krpch.h>

#include "Camera.h"
#include "RendererAPI.h"

namespace Kraken {
    class Renderer {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const Camera &camera);
        static void EndScene();

        static void SetShader(const Ref<Shader>& shader);
        static void Submit(const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

        static void OnWindowResize(uint32_t width, uint32_t height);
    private:
        struct RenderData
        {
            Shader* Shader; // This object does not own the shader
        };

        static Scope<RenderData> s_RenderData;
    };    
}
