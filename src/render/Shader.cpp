#include "Shader.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include "ShaderManager.h"
#include "UserDebugInfo.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	this->Program = ShaderManager::instance().get_shader_program(vertexPath, fragmentPath);
}

void Shader::Use()
{
	glUseProgram(reinterpret_cast<size_t>(this->Program.get()));
}

GLuint Shader::program()
{
	return reinterpret_cast<size_t>(this->Program.get());
}
