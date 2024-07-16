//
// Created by Sebastian Snoer on 16/07/2024.
//

#include "VKShader.h"

namespace Kraken {
    VKShader::VKShader(const std::string &vertexSrc, const std::string &fragmentSrc) {
    }

    VKShader::VKShader(AssetSpecification &specs) {
    }

    VKShader::~VKShader() {
    }

    void VKShader::Bind() const {
    }

    void VKShader::Unbind() const {
    }

    void VKShader::SetInt(const std::string &name, int value) {
    }

    void VKShader::SetIntArray(const std::string &name, int *values, uint32_t count) {
    }

    void VKShader::SetFloat(const std::string &name, float value) {
    }

    void VKShader::SetFloat2(const std::string &name, const glm::vec2 &value) {
    }

    void VKShader::SetFloat3(const std::string &name, const glm::vec3 &value) {
    }

    void VKShader::SetFloat4(const std::string &name, const glm::vec4 &value) {
    }

    void VKShader::SetMat4(const std::string &name, const glm::mat4 &value) {
    }

    void VKShader::SetColor(const std::string &name, const Color &value) {
    }
}
