#include "Light.h"

#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ImportObj.h"

Light::Light() : shader("vertexSimple.glsl", "fragmentSimple.glsl")
{
	std::vector<glm::vec3> vertices;
	OBJLoader::loadVertices("models/sphere.obj", vertices);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	this->triCount = (int)vertices.size();
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(glm::vec3) * vertices.size()), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
}

Light::~Light()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Light::render(float /*time*/, const glm::mat4 &PV)
{
	model = glm::translate(glm::mat4(1.f), glm::vec3(pos));
	//model = glm::rotate(model, glm::radians((GLfloat)(time + addr) * 50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f));

	auto PVM = PV * model;
	shader.Use();
	GLint transformLocation = glGetUniformLocation(shader.Program, "PVM");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(PVM));
	// Обновляем цвет формы
	GLint vertexColorLocation = glGetUniformLocation(shader.Program, "simpleColor");
	glUniform4f(vertexColorLocation, 1.f, 1.f, 1.0f, 1.0f);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, triCount);
	glBindVertexArray(0);
}
