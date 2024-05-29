//
// Created by sebsn on 30-04-2024.
//

#include "Shader.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "RenderCommand.h"
#include "Kraken/Utils/Files.h"

namespace Kraken {
    shaderc_shader_kind ShaderUtils::ShaderTypeToShaderC(const ShaderType t) {
        switch (t) {
            case VERTEX_SHADER: return shaderc_glsl_vertex_shader;
            case FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
        }

        KRC_ASSERT(false, "Unkown shader type!")
        return static_cast<shaderc_shader_kind>(0);
    }

    const char * ShaderUtils::ShaderTypeCachedOpenGLFileExtension(ShaderType t) {
        switch (t) {
            case VERTEX_SHADER: return ".cached_opengl.vert";
            case FRAGMENT_SHADER: return ".cached_opengl.frag";
        }

        KRC_ASSERT(false, "Unkown shader type!")
        return "";
    }

    const char * ShaderUtils::ShaderTypeCachedVulkanFileExtension(ShaderType t) {
        switch (t) {
            case VERTEX_SHADER: return ".cached_vulkan.vert";
            case FRAGMENT_SHADER: return ".cached_vulkan.frag";
        }

        KRC_ASSERT(false, "Unkown shader type!")
        return "";
    }

    const char * ShaderUtils::ShaderTypeToShortString(ShaderType t) {
        switch (t) {
            case VERTEX_SHADER: return "VERT";
            case FRAGMENT_SHADER: return "FRAG";
        }

        KRC_ASSERT(false, "Unkown shader type!")
        return "";
    }

    ShaderType ShaderUtils::StringToShaderType(std::string& s) {
        if (s == "vertex")
            return VERTEX_SHADER;
        if (s == "fragment" || s == "pixel")
            return FRAGMENT_SHADER;

        KRC_ASSERT(false, "Unknown shader type!");
        return static_cast<ShaderType>(0);
    }

    std::unordered_map<ShaderType, std::vector<uint32_t>> ShaderUtils::CompileOrGetVulkanBinaries(
        const std::unordered_map<ShaderType, std::string> &shaderSources, Identifier identifier) {
        // Output
        std::unordered_map<ShaderType, std::vector<uint32_t>> shaderData;
        shaderData.clear();
        
        // Compiler options
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
        options.SetOptimizationLevel(shaderc_optimization_level_performance);

        std::filesystem::path cacheDirectory = Files::GetCacheDir() + "/shader/" + identifier.domain + "/";

        for(auto&& [ stage, source ] : shaderSources) {
            std::filesystem::path cachedPath = cacheDirectory / (identifier.name + ShaderTypeCachedVulkanFileExtension(stage));

            if (std::ifstream in(cachedPath, std::ios::in | std::ios::binary); in.is_open()) {
                in.seekg(0, std::ios::end);
                auto size = in.tellg();
                in.seekg(0, std::ios::beg);

                auto& data = shaderData[stage];
                data.resize(size / sizeof(uint32_t));
                in.read((char*)data.data(), size);
            } else {
                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source,
                    ShaderTypeToShaderC(stage),
                    (identifier.ToString() + ":" + ShaderTypeToShortString(stage)).c_str(),
                    options);
                if(module.GetCompilationStatus() != shaderc_compilation_status_success) {
                    KRC_ERROR(module.GetErrorMessage());
                    KRC_ASSERT(false);
                }

                shaderData[stage] = std::vector(module.cbegin(), module.cend());

                if (std::ofstream out(cachedPath, std::ios::out | std::ios::binary); out.is_open()) {
                    auto& data = shaderData[stage];
                    out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                    out.flush();
                    out.close();
                }
            }
        }

        return shaderData;
    }

    std::unordered_map<ShaderType, std::vector<uint32_t>> ShaderUtils::CompileOrGetOpenGLBinaries(
        const std::unordered_map<ShaderType, std::vector<uint32_t>> &vulkanSPIRV, Identifier identifier) {
        // Output
        std::unordered_map<ShaderType, std::vector<uint32_t>> shaderData;
		std::unordered_map<ShaderType, std::string> openGLSourceCode;
        shaderData.clear();

        // Compiler options
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

        std::filesystem::path cacheDirectory = Files::GetCacheDir() + "/shader/" + identifier.domain + "/";

        for(auto&& [ stage, spirv ] : vulkanSPIRV) {
            std::filesystem::path cachedPath = cacheDirectory / (identifier.name + ShaderTypeCachedOpenGLFileExtension(stage));

            if (std::ifstream in(cachedPath, std::ios::in | std::ios::binary); in.is_open()) {
                in.seekg(0, std::ios::end);
                auto size = in.tellg();
                in.seekg(0, std::ios::beg);

                auto& data = shaderData[stage];
                data.resize(size / sizeof(uint32_t));
                in.read((char*)data.data(), size);
            } else {
                spirv_cross::CompilerGLSL glslCompiler(spirv);
                openGLSourceCode[stage] = glslCompiler.compile();
                auto& source = openGLSourceCode[stage];

                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source,
                    ShaderTypeToShaderC(stage),
                    (identifier.ToString() + ":" + ShaderTypeToShortString(stage)).c_str()/*,
                    options*/);
                if(module.GetCompilationStatus() != shaderc_compilation_status_success) {
                    KRC_ERROR(module.GetErrorMessage());
                    KRC_ASSERT(false);
                }

                shaderData[stage] = std::vector(module.cbegin(), module.cend());

                if (std::ofstream out(cachedPath, std::ios::out | std::ios::binary); out.is_open()) {
                    auto& data = shaderData[stage];
                    out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                    out.flush();
                    out.close();
                }
            }
        }

        return shaderData;
    }
}
