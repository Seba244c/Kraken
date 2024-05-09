//
// Created by sebsn on 07-05-2024.
//

#include "Renderer.h"

#include "RenderCommand.h"

namespace Kraken {
	Scope<Renderer::RenderData> Renderer::s_RenderData = CreateScope<Renderer::RenderData>();

    void Renderer::Init() {
    }

    void Renderer::Shutdown() {
    }

    void Renderer::BeginScene(const Camera& camera) {
        s_RenderData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
        //camera.GetFBO()->Bind();
    }

    void Renderer::EndScene() {

    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray> &vertexArray) {
        shader->Bind();
        shader->SetMat4("u_mProjectionView", s_RenderData->ViewProjectionMatrix);

        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::OnWindowResize(const uint32_t width, const uint32_t height) {
        RenderCommand::SetViewport(0, 0, width, height);
    }
}
