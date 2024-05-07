//
// Created by sebsn on 07-05-2024.
//

#include "Renderer.h"

#include "RenderCommand.h"

namespace Kraken {
    void Renderer::Init() {
    }

    void Renderer::Shutdown() {
    }

    void Renderer::BeginScene() {
    }

    void Renderer::EndScene() {
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray> &vertexArray) {
        shader->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}
