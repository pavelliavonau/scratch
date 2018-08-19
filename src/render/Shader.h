#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <GL/glew.h>

class Shader
{
public:
	GLuint Program;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();
	void Use();
};

#endif // SHADER_H
