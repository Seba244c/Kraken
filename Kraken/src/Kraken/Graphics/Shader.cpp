//
// Created by sebsn on 30-04-2024.
//

#include "Shader.h"

#include "Renderer.h"
#include "Kraken/Platform/OpenGL/OpenGLShader.h"

namespace Kraken {
    Ref<Shader> Shader::Create(const std::string& filepath) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:    KRC_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(filepath);
        }

        KRC_ASSERT(false, "Unkown RenderAPI!");
        return nullptr;
    }

    void ShaderLibrary::Add(const std::string &name, const Ref<Shader> &shader) {
        KRC_ASSERT(!Exists(name), "Shader already exists");
        m_Shaders[name] = shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string &name, const std::string &filepath) {
        auto shader = Shader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string &name) {
        KRC_ASSERT(Exists(name), "Shader not found!");
        return m_Shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string &name) const {
        return m_Shaders.find(name) != m_Shaders.end();
    }
}
