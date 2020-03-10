#ifndef SHADER_H
#define SHADER_H

#include <memory>
#include <string>

#include <GL/glew.h>

class Shader
{
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void Use();
	GLuint program();

private:
	std::shared_ptr<GLuint> Program;
};

#endif // SHADER_H
