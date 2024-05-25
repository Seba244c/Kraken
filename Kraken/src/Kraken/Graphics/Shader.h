//
// Created by sebsn on 30-04-2024.
//

#pragma once
#include <krpch.h>

#include "Kraken/Core/Asset.h"
#include <shaderc/shaderc.h>

namespace Kraken {
    enum ShaderType {
        FRAGMENT_SHADER = 1,
        VERTEX_SHADER,
    };

    class ShaderUtils {
    public:
        static shaderc_shader_kind ShaderTypeToShaderC(ShaderType t);
        static const char* ShaderTypeCachedOpenGLFileExtension(ShaderType t);
        static const char* ShaderTypeCachedVulkanFileExtension(ShaderType t);
        static const char* ShaderTypeToShortString(ShaderType t);

        static std::unordered_map<ShaderType, std::vector<uint32_t>> CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& shaderSources, Identifier identifier);
        static std::unordered_map<ShaderType, std::vector<uint32_t>> CompileOrGetOpenGLBinaries(const std::unordered_map<ShaderType, std::vector<uint32_t>>& vulkanSPIRV, Identifier identifier);
    };

    class Shader : public Asset {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetInt(const std::string& name, int value) = 0;
        virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
        virtual void SetFloat(const std::string& name, float value) = 0;
        virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
        virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
        virtual void SetColor(const std::string& name, const Color& value) = 0;
    };

    class ShaderLibrary
    {
    public:
        void Add(const Identifier& identifier, const Ref<Shader>& shader);
        Ref<Shader> Load(const Identifier& identifier, const std::string& vertexSrc, const std::string& fragSource);
        Ref<Shader> Load(const Identifier& identifier);
        Ref<Shader> Get(const Identifier& identifier);
        
        bool Exists(const Identifier& identifier) const;
    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };
}
