#ifndef TEXTUREDBOX_H
#define TEXTUREDBOX_H

#include "SceneObject.h"
#include <GL/glew.h>
#include <Shader.h>

class TexturedBox : public SceneObject
{
public:
	TexturedBox();
	~TexturedBox();

	void render(double time, const glm::mat4 &) override;

	float hOffset = 0.0;
	float vOffset = 0.0;

private:
	GLuint VBO, VAO, EBO;
	GLuint texture;
	GLuint texture1;
	Shader shader;
};

#endif // TEXTUREDBOX_H
