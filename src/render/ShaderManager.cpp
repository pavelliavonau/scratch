#include "ShaderManager.h"

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "UserDebugInfo.h"

ShaderManager &ShaderManager::instance()
{
	static ShaderManager instance;
	return instance;
}

GLuint &ShaderManager::get_vertex_shader(const std::string &shader_source_path)
{
	return this->get_shader(shader_source_path, GL_VERTEX_SHADER);
}

GLuint &ShaderManager::get_fragment_shader(const std::string &shader_source_path)
{
	return this->get_shader(shader_source_path, GL_FRAGMENT_SHADER);
}

void ShaderManager::clean_shaders()
{
	for(auto shader : this->shaders)
		glDeleteShader(shader.second);
	this->shaders.clear();
}

void ShaderManager::set_shaders_root(const std::string &path_prefix)
{
	this->shaders_root = path_prefix;
}

GLuint &ShaderManager::get_shader(const std::string &shader_source_path, int shader_type)
{
	auto key = shader_source_path;
	auto it = this->shaders.find(key);
	if (it == this->shaders.end())
	{
		auto full_shader_source_path = this->shaders_root + shader_source_path;
		std::string shaderCode = readShaderSource(full_shader_source_path);
		const GLchar* fShaderCode = shaderCode.c_str();
		UserDebugInfo udi("path: " + full_shader_source_path);
		std::cout << "Compilation " << full_shader_source_path << " ...";
		GLuint shader = glCreateShader(shader_type);
		glShaderSource(shader, 1, &fShaderCode, NULL);
		glCompileShader(shader);
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glDeleteShader(shader);
			throw std::runtime_error("shader compilation error in " + shader_source_path + "\n");
		}
		else
			std::cout << " DONE" << std::endl;
		shaders.insert(std::make_pair(key, shader));
	}
	return shaders.at(key);
}

std::string ShaderManager::readShaderSource(const std::string &path) const
{
	std::string code;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::badbit);
	std::cout << "Reading " << path << " ...";

	{
		// Открываем файлы
		shaderFile.open(path);
		std::stringstream shaderStream;
		// Считываем данные в потоки
		shaderStream << shaderFile.rdbuf();
		// Закрываем файлы
		shaderFile.close();
		// Преобразовываем потоки в массив GLchar
		code = shaderStream.str();
		std::cout << " DONE" << std::endl;
	}

	return code;
}
