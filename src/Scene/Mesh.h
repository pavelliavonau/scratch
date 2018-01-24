#ifndef MESH_H
#define MESH_H

#include "SceneObject.h"
#include <GL/glew.h>
#include <Shader.h>
#include <vector>

class Mesh : public SceneObject
{
public:
	Mesh(const std::vector<GLfloat>& vertices);
	~Mesh();

	void render(float time, const glm::mat4 &PV) override;

	bool blinking = false;

private:
	GLuint VBO, VAO;
	Shader shader;
	int triCount = 0;
};

#endif // MESH_H
