#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

extern std::string debug_user_param;

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	std::string vertexCode = readShaderSource(vertexPath);
	std::string fragmentCode = readShaderSource(fragmentPath);
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	// 2. Сборка шейдеров
	GLuint vertex, fragment;
	GLint success;

	std::cout << "Compilation " << vertexPath << " ...";
	debug_user_param = "path: " + std::string(vertexPath);
	// Вершинный шейдер
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	debug_user_param = "";
	// Если есть ошибки - вывести их
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glDeleteShader(vertex);
		throw std::runtime_error("vertex shader compilation error in " + std::string(vertexPath) + "\n");
	}
	else
		std::cout << " DONE" << std::endl;

	std::cout << "Compilation " << fragmentPath << " ...";
	debug_user_param = "path: " + std::string(fragmentPath);
	// Вершинный шейдер
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	debug_user_param = "";
	// Если есть ошибки - вывести их
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glDeleteShader(fragment);
		throw std::runtime_error("fragment shader compilation error in " + std::string(fragmentPath) + "\n");
	}
	else
		std::cout << " DONE" << std::endl;

	// Шейдерная программа
	this->Program = glCreateProgram();
	debug_user_param = "path: " + std::string(vertexPath) + "\n" + std::string(fragmentPath);
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);
	debug_user_param = "";
	//Если есть ошибки - вывести их
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if(!success)
	{
		glDeleteProgram(this->Program);
		throw std::runtime_error("linking shader program error");
	}

	// Удаляем шейдеры, поскольку они уже в программу и нам больше не нужны.
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteProgram(this->Program);
}

std::string Shader::readShaderSource(const GLchar* path) const
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

void Shader::Use()
{
	glUseProgram(this->Program);
}
