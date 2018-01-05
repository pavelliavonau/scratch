#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	std::string vertexCode = readShaderSource(vertexPath);
	std::string fragmentCode = readShaderSource(fragmentPath);
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	// 2. Сборка шейдеров
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	std::cout << "Compilation " << vertexPath << " ...";
	// Вершинный шейдер
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Если есть ошибки - вывести их
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
		std::cout << " DONE" << std::endl;

	std::cout << "Compilation " << fragmentPath << " ...";
	// Вершинный шейдер
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// Если есть ошибки - вывести их
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
		std::cout << " DONE" << std::endl;

	// Шейдерная программа
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);
	//Если есть ошибки - вывести их
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Удаляем шейдеры, поскольку они уже в программу и нам больше не нужны.
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

std::string Shader::readShaderSource(const GLchar* path) const
{
	std::string code;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::badbit);
	std::cout << "Reading " << path << " ...";
	try
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
	catch(std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return code;
}

void Shader::Use()
{
	glUseProgram(this->Program);
}
