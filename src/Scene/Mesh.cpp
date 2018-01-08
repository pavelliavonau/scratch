#include "Mesh.h"
#include <cmath>

Mesh::Mesh(const std::vector<GLfloat>& vertices)
	:shader("shaders/vertexSimple.glsl", "shaders/fragmentSimple.glsl")
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Mesh::render(double time, const glm::mat4&)
{
	// Draw our first triangle
	shader.Use();
	// Обновляем цвет формы
	GLdouble greenValue = ((this->blinking ? sin(time) : 1) / 2) + 0.5;
	GLint vertexColorLocation = glGetUniformLocation(shader.Program, "simpleColor");
	glUniform4f(vertexColorLocation, 0.0f, static_cast<GLfloat>(greenValue), 0.0f, 1.0f);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
