#ifndef TEXTUREDMESH_H
#define TEXTUREDMESH_H

#include "SceneObject.h"
#include <GL/glew.h>
#include <Shader.h>
#include <vector>

class TexturedMesh : public SceneObject
{
public:
	/*
	 * strides
	 *    stride = vec3 (vertex) + vec3 (color) + vec2 (UV)
	 * */
	TexturedMesh(const std::vector<GLfloat>& data);
	~TexturedMesh();

	void render(double time, const glm::mat4 &PVM) override;

	bool blinking = false;
	glm::vec3 pos = {0.f, 0.f, 0.f};

private:
	GLuint VBO, VAO;
	GLuint texture;
	Shader shader;
	int triCount = 0;
	glm::mat4 model = glm::mat4(1.0f);
};

#endif // TEXTUREDMESH_H
