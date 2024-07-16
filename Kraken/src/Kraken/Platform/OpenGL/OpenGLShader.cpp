//
// Created by sebsn on 30-04-2024.
//

#include "OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <shaderc/shaderc.hpp>

#include "Kraken/Utils/Files.h"

namespace Kraken {
	uint32_t OpenGLShader::ShaderTypeToOpenGL(ShaderType t) {
		switch (t) {
			case VERTEX_SHADER: return GL_VERTEX_SHADER;
			case FRAGMENT_SHADER: return GL_FRAGMENT_SHADER;
		}

		KRC_ASSERT(false, "Unkown shader type!")
		return static_cast<shaderc_shader_kind>(0);
	}

    OpenGLShader::OpenGLShader(AssetSpecification& specs)  {
        KR_PROFILE_FUNCTION();

        std::string source = specs.ToString();
        const auto sources = ShaderUtils::ProcessAssetSourceCode(source);

        const auto vulkanSPRIV =
            ShaderUtils::CompileOrGetVulkanBinaries(sources, specs.GetIdentifier());

        m_OpenGLSPIRV =
            ShaderUtils::CompileOrGetOpenGLBinaries(vulkanSPRIV, specs.GetIdentifier());

        CreateProgram(specs.GetIdentifier());
    }

    OpenGLShader::OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc) {
        KR_PROFILE_FUNCTION();

    	std::unordered_map<ShaderType, std::string> sources;
    	sources[VERTEX_SHADER] = vertexSrc;
    	sources[FRAGMENT_SHADER] = fragmentSrc;

        const Identifier i = {"Anonymous", "Shader"};
	    const auto vulkanSPRIV =
	    	ShaderUtils::CompileOrGetVulkanBinaries(sources, i);

    	m_OpenGLSPIRV =
    		ShaderUtils::CompileOrGetOpenGLBinaries(vulkanSPRIV, i);

    	CreateProgram(i);
    }

    void OpenGLShader::CreateProgram(const Identifier& identifier) {
        KR_PROFILE_FUNCTION();

		const GLuint program = glCreateProgram();

    	// Create and Attach all shaders
    	std::vector<GLuint> shaderIDs;
    	for(auto&& [stage, spirv] : m_OpenGLSPIRV) {
    		KRC_TRACE("Creating Shader: " + identifier.ToString() + ":" + ShaderUtils::ShaderTypeToShortString(stage));

    		GLuint shaderID = shaderIDs.emplace_back(glCreateShader(ShaderTypeToOpenGL(stage)));

    		glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
    		glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
    		glAttachShader(program, shaderID);
    	}

        glLinkProgram(program);

    	// Check for erros
    	GLint isLinked;
    	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    	if(isLinked == GL_FALSE) {
    		GLint maxLength;
    		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    		std::vector<GLchar> infoLog(maxLength);
    		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
    		KRC_ERROR("Shader linking failed ({0}):\n{1}", identifier.ToString(), infoLog.data());

    		glDeleteProgram(program);

    		for (const auto id : shaderIDs)
    			glDeleteShader(id);
    	}

    	// Delete shaders (they are now linked)
    	for(const auto shaderID : shaderIDs) {
    		glDetachShader(program, shaderID);
    		glDeleteShader(shaderID);
    	}

    	m_RendererID = program;
    }

    OpenGLShader::~OpenGLShader() {
        KR_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::Bind() const {
        KR_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const {
        KR_PROFILE_FUNCTION();

		glUseProgram(0);
    }

    void OpenGLShader::SetInt(const std::string &name, int value) {
        KR_PROFILE_FUNCTION();

    	const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform1i(location, value);
    }

    void OpenGLShader::SetIntArray(const std::string &name, int *values, uint32_t count) {
        KR_PROFILE_FUNCTION();

    	const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform1iv(location, count, values);
    }

    void OpenGLShader::SetFloat(const std::string &name, float value) {
        KR_PROFILE_FUNCTION();

    	const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform1f(location, value);
    }

	void OpenGLShader::SetFloat2(const std::string &name, const glm::vec2 &value) {
        KR_PROFILE_FUNCTION();

    	const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform2f(location, value.x, value.y);
    }

	void OpenGLShader::SetFloat3(const std::string &name, const glm::vec3 &value) {
        KR_PROFILE_FUNCTION();

    	const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform3f(location, value.x, value.y, value.z);
    }

	void OpenGLShader::SetFloat4(const std::string &name, const glm::vec4 &value) {
        KR_PROFILE_FUNCTION();

    	const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetMat4(const std::string &name, const glm::mat4 &value) {
        KR_PROFILE_FUNCTION();

    	const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::SetColor(const std::string &name, const Color &value) {
        KR_PROFILE_FUNCTION();

	    const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform4f(location, value.r, value.g, value.b, value.a);
    }
} // Kraken
