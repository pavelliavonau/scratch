#ifndef BLINKINGTRIANGLE_H
#define BLINKINGTRIANGLE_H

#include "SceneObject.h"
#include <GL/glew.h>
#include <Shader.h>
#include <vector>

class Mesh : public SceneObject
{
public:
	Mesh(const std::vector<GLfloat>& vertices);
	~Mesh();

	void render(double time) override;

	bool blinking = false;

private:
	GLuint VBO, VAO;
	Shader shader;
};

#endif // BLINKINGTRIANGLE_H
