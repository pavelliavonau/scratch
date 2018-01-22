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

private:
	std::string readShaderSource(const GLchar* path) const;
};

#endif // SHADER_H
