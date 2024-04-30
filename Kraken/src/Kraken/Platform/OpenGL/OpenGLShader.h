//
// Created by sebsn on 30-04-2024.
//

#ifndef KR_OPENGLSHADER_H
#define KR_OPENGLSHADER_H
#include "Kraken/Graphics/Shader.h"

namespace Kraken {
    class OpenGLShader : public Shader{
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader() override;

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
    private:
        uint32_t m_RendererID;
        std::string m_FilePath;
    };
} // Kraken

#endif //KR_OPENGLSHADER_H
