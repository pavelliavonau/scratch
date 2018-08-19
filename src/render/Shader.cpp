#include "Shader.h"

#include <string>
#include <stdexcept>
#include "UserDebugInfo.h"
#include "ShaderManager.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	GLuint vertex = ShaderManager::instance().get_vertex_shader(vertexPath);
	GLuint fragment = ShaderManager::instance().get_fragment_shader(fragmentPath);

	{
		UserDebugInfo udi("path: " + std::string(vertexPath) + "\n" + std::string(fragmentPath));
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);
		GLint success;
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if(!success)
		{
			glDeleteProgram(this->Program);
			throw std::runtime_error("linking shader program error");
		}
	}
}

Shader::~Shader()
{
	glDeleteProgram(this->Program);
}

void Shader::Use()
{
	glUseProgram(this->Program);
}
