//
// Created by sebsn on 30-04-2024.
//

#include "Shader.h"

namespace Kraken {
    void ShaderLibrary::Add(const std::string &name, const Ref<Shader> &shader) {
        KRC_ASSERT(!Exists(name), "Shader already exists");
        m_Shaders[name] = shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string &name, const std::string &filepath) {
        //auto shader = Shader::Create(filepath);
        //Add(name, shader);
       // return shader;
        return nullptr;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string &name) {
        KRC_ASSERT(Exists(name), "Shader not found!");
        return m_Shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string &name) const {
        return m_Shaders.find(name) != m_Shaders.end();
    }
}
