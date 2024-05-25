//
// Created by sebsn on 07-05-2024.
//

#include "Renderer.h"

#include "RenderCommand.h"

namespace Kraken {
	Scope<Renderer::RenderData> Renderer::s_RenderData = CreateScope<Renderer::RenderData>();

    struct RendererData {
        struct CameraData {
            glm::mat4 ViewProjection;
        };
        CameraData CameraBuffer;
        Ref<UniformBuffer> CameraUniformBuffer;
    };

    static RendererData s_Data;
    void Renderer::Init() {
		s_Data.CameraUniformBuffer = RenderCommand::CreateUniformBuffer(sizeof(RendererData::CameraData), 0);
    }

    void Renderer::Shutdown() {
    }

    void Renderer::BeginScene(const Camera& camera) {
        s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));
        //camera.GetFBO()->Bind();
    }

    void Renderer::EndScene() {

    }

    void Renderer::SetShader(const Ref<Shader>& shader) {
        shader->Bind();
        //shader->SetMat4("u_mViewProjection", s_RenderData->ViewProjectionMatrix);
        s_RenderData->Shader = &*shader;
    }


    void Renderer::Submit(const Ref<VertexArray> &vertexArray, const glm::mat4& transform) {
        //s_RenderData->Shader->SetMat4("u_mModel", transform);
        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::OnWindowResize(const uint32_t width, const uint32_t height) {
        RenderCommand::SetViewport(0, 0, width, height);
    }
}
