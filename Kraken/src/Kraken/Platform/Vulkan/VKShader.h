//
// Created by sebsn on 15-07-2024.
//

#pragma once
#include <krpch.h>

#include "Kraken/Graphics/Shader.h"

namespace Kraken {
    class VKShader final : public Shader{
    public:
        VKShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        explicit VKShader(AssetSpecification& specs);
        ~VKShader() override;

        void Bind() const override;
        void Unbind() const override;

        void SetInt(const std::string &name, int value) override;
        void SetIntArray(const std::string &name, int *values, uint32_t count) override;
        void SetFloat(const std::string &name, float value) override;
        void SetFloat2(const std::string &name, const glm::vec2 &value) override;
        void SetFloat3(const std::string &name, const glm::vec3 &value) override;
        void SetFloat4(const std::string &name, const glm::vec4 &value) override;
        void SetMat4(const std::string &name, const glm::mat4 &value) override;
        void SetColor(const std::string &name, const Color &value) override;
    };
} // Kraken