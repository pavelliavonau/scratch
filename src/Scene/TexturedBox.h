#ifndef TEXTUREDBOX_H
#define TEXTUREDBOX_H

#include "SceneObject.h"
#include <GL/glew.h>
#include <Shader.h>
#include <vector>

class TexturedBox : public SceneObject
{
public:
	TexturedBox(const std::vector<GLfloat>& data);
	~TexturedBox();

	void render(float time, const glm::mat4 &PV) override;

	float hOffset = 0.0;
	float vOffset = 0.0;

private:
	GLuint VBO, VAO, EBO;
	GLuint texture;
	GLuint texture1;
	Shader shader;
};

#endif // TEXTUREDBOX_H
