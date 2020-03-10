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

GLuint ShaderManager::get_vertex_shader(const std::string &shader_source_path)
{
	return this->get_shader(shader_source_path, GL_VERTEX_SHADER);
}

GLuint ShaderManager::get_fragment_shader(const std::string &shader_source_path)
{
	return this->get_shader(shader_source_path, GL_FRAGMENT_SHADER);
}

std::shared_ptr<GLuint> ShaderManager::get_shader_program(const std::string &vertexPath,
                                                          const std::string &fragmentPath)
{
	auto key = std::make_pair(vertexPath, fragmentPath);
	auto it = this->programs.find(key);
	if (it == this->programs.end()) {
		UserDebugInfo udi("path: " + std::string(vertexPath) + "\n" + std::string(fragmentPath));
		GLuint vertex = this->get_vertex_shader(vertexPath);
		GLuint fragment = this->get_fragment_shader(fragmentPath);
		GLuint Program = glCreateProgram();
		glAttachShader(Program, vertex);
		glAttachShader(Program, fragment);
		std::cout << "Link Program "
		          << std::string(vertexPath) + " + " + std::string(fragmentPath) + " == " << Program
		          << "\n";
		glLinkProgram(Program);
		GLint success;
		glGetProgramiv(Program, GL_LINK_STATUS, &success);
		if (!success) {
			glDeleteProgram(Program);
			throw std::runtime_error("linking shader program error");
		}

		std::shared_ptr<GLuint> pointer(reinterpret_cast<GLuint *>(Program),
		                                [this, key](GLuint *program) {
			                                std::cout << "Delete program "
			                                          << reinterpret_cast<size_t>(program)
			                                          << "...\n";
			                                glDeleteProgram(reinterpret_cast<size_t>(program));
			                                if (!exiting) {
				                                auto it = this->programs.find(key);
				                                if (it != this->programs.end())
					                                this->programs.erase(it);
			                                }
		                                });

		programs.insert({key, pointer});
	}
	return programs.at(key);
}

void ShaderManager::set_shaders_root(const std::string &path_prefix)
{
	this->shaders_root = path_prefix;
}

GLuint ShaderManager::get_shader(const std::string &shader_source_path, int shader_type)
{
	auto key = shader_source_path;
	auto it = this->shaders.find(key);
	if (it == this->shaders.end())
	{
		auto full_shader_source_path = this->shaders_root + shader_source_path;
		std::string shaderCode = readShaderSource(full_shader_source_path);
		const GLchar* fShaderCode = shaderCode.c_str();
		UserDebugInfo udi("path: " + full_shader_source_path);
		GLuint shader = glCreateShader(shader_type);
		std::cout << "Compilation " << full_shader_source_path << " == " << shader << " "
		          << " ...";
		glShaderSource(shader, 1, &fShaderCode, NULL);
		glCompileShader(shader);
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glDeleteShader(shader);
			throw std::runtime_error("shader compilation error in " + shader_source_path + "\n");
		} else {
			std::cout << " DONE" << std::endl;
		}

		std::shared_ptr<GLuint> pointer(reinterpret_cast<GLuint *>(shader),
		                                [this, key](GLuint *shader) {
			                                std::cout << "Deleting shader "
			                                          << reinterpret_cast<size_t>(shader)
			                                          << "...\n";
			                                glDeleteShader(reinterpret_cast<size_t>(shader));
			                                if (!exiting) {
				                                auto it = this->shaders.find(key);
				                                if (it != this->shaders.end())
					                                this->shaders.erase(it);
			                                }
		                                });

		shaders.insert({key, pointer});
	}
	return reinterpret_cast<size_t>(shaders.at(key).get());
}

std::string ShaderManager::readShaderSource(const std::string &path) const
{
	std::string code;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::badbit);
	std::cout << "Reading " << path << " ...";

	{
		shaderFile.open(path);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		code = shaderStream.str();
		std::cout << " DONE" << std::endl;
	}

	return code;
}
