#include "TexturedMesh.h"
#include <cmath>
#include <SOIL.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

TexturedMesh::TexturedMesh(const std::vector<GLfloat>& data)
	:shader("shaders/vertexColor.glsl", "shaders/fragmentColor.glsl")
{
	const int STRIDE_SIZE = 8;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	this->triCount = (int)data.size() / STRIDE_SIZE;
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(GLfloat) * data.size()), data.data(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE_SIZE * sizeof(GLfloat), static_cast<GLvoid*>(0));
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE_SIZE * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIDE_SIZE * sizeof(GLfloat), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int imageWidth, imageHeight;
	unsigned char* image1 = SOIL_load_image("textures/awesomeface.png", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);
}

TexturedMesh::~TexturedMesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void TexturedMesh::render(double time, const glm::mat4& PV)
{
	size_t addr = reinterpret_cast<size_t>(this) & 0xFF;
	model = glm::translate(glm::mat4(1.f), glm::vec3(pos));
	model = glm::rotate(model, glm::radians((GLfloat)(time + addr) * 50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

	auto PVM = PV * model;
	shader.Use();
	GLint transformLocation = glGetUniformLocation(shader.Program, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(PVM));

	GLint useTextureRate = glGetUniformLocation(shader.Program, "useTextureRate");
	glUniform1f(useTextureRate, 0.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader.Program, "ourTexture1"), 0);

	// Draw container
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, triCount);
	glBindVertexArray(0);
}
