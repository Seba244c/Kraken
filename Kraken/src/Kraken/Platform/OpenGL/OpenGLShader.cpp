//
// Created by sebsn on 30-04-2024.
//

#include "OpenGLShader.h"
#include <glad/gl.h>

#include <glm/gtc/type_ptr.hpp>

#include "Kraken/Utils/Files.h"

namespace Kraken {
    OpenGLShader::OpenGLShader(const std::string &filepath) : m_FilePath(filepath) {
    	auto src = Files::ReadFile(filepath);
		const std::string delimiter = "#type fragment";
	    const std::string vertexSrc = src.substr(0, src.find(delimiter));
	    const std::string fragmentSrc = src.substr(src.find(delimiter)+delimiter.length());
    	
		//OpenGLShader(vertexSrc, fragmentSrc);
    	m_RendererID = 0;
    }

    OpenGLShader::OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc) {
    	// Create an empty vertex shader handle
	    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar *source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
			
			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			KRC_ERROR("{0}", infoLog.data());
			KRC_ASSERT(false, "Shader Compilation Failure!")
			return;
		}

		// Create an empty fragment shader handle
	    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
			
			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			KRC_ERROR("{0}", infoLog.data());
			KRC_ASSERT(false, "Shader Compilation Failure!")
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		// Link our program
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);
			
			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			KRC_ERROR("{0}", infoLog.data());
			KRC_ASSERT(false, "Shader Link Failure!")
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
    }

    OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::Bind() const {
		glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const {
		glUseProgram(0);
    }

    void OpenGLShader::SetInt(const std::string &name, int value) {
    	const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform1i(location, value);
    }

    void OpenGLShader::SetIntArray(const std::string &name, int *values, uint32_t count) {
    	const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform1iv(location, count, values);
    }

    void OpenGLShader::SetFloat(const std::string &name, float value) {
    	const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform1f(location, value);
    }

	void OpenGLShader::SetFloat2(const std::string &name, const glm::vec2 &value) {
    	const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform2f(location, value.x, value.y);
    }

	void OpenGLShader::SetFloat3(const std::string &name, const glm::vec3 &value) {
    	const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform3f(location, value.x, value.y, value.z);
    }

	void OpenGLShader::SetFloat4(const std::string &name, const glm::vec4 &value) {
    	const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetMat4(const std::string &name, const glm::mat4 &value) {
    	const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
	
    void OpenGLShader::SetColor(const std::string &name, const Color &value) {
	    const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    	glUniform4f(location, value.r, value.g, value.b, value.a);
    }
} // Kraken