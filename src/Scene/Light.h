#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>

#include "SceneObject.h"
#include "Shader.h"

class Light : public SceneObject
{
public:
	Light();
	~Light();

	// SceneObject interface
public:
	void render(float time, const glm::mat4 &PV) override;
	glm::vec3 pos = {1.2f, 1.0f, 2.0f};

private:
	GLuint VBO, VAO;
	GLuint texture;
	Shader shader;
	int triCount = 0;
	glm::mat4 model = glm::mat4(1.0f);
};

#endif // LIGHT_H
