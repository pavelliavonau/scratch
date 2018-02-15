#include "TexturedMesh.h"
#include <cmath>
#include <SOIL.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

TexturedMesh::TexturedMesh(const std::vector<glm::vec3>& vertexData, const std::vector<glm::vec2>& uvData, const std::vector<glm::vec3>& normalsData)
	:shader("shaders/vertexColor.glsl", "shaders/fragmentColor.glsl")
{
	this->triCount = static_cast<int>(vertexData.size());

	{
		glGenVertexArrays(1, &VAO);

		glGenBuffers(1, &VBO_vert);
		glGenBuffers(1, &VBO_uv);
		glGenBuffers(1, &VBO_norm);

		glBindVertexArray(VAO);

		{// vertex stream
			glBindBuffer(GL_ARRAY_BUFFER, VBO_vert);
			glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(glm::vec3) * vertexData.size()), vertexData.data(), GL_STATIC_DRAW);

			// Position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		{// uv stream
			glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);

			glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(glm::vec2) * uvData.size()), uvData.data(), GL_STATIC_DRAW);

			// TexCoord attribute
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid*>(0));
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		{// normals stream
			glBindBuffer(GL_ARRAY_BUFFER, VBO_norm);

			glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(glm::vec3) * normalsData.size()), normalsData.data(), GL_STATIC_DRAW);

			// TexCoord attribute
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	}

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
	glDeleteBuffers(1, &VBO_vert);
	glDeleteBuffers(1, &VBO_uv);
	glDeleteBuffers(1, &VBO_norm);
}

void TexturedMesh::render(float time, const glm::mat4& PV)
{
	size_t addr = reinterpret_cast<size_t>(this) & 0xFF;
	model = glm::translate(glm::mat4(1.f), glm::vec3(pos));
	model = glm::rotate(model, glm::radians((GLfloat)(time + addr) * 50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

	shader.Use();
	GLint projectionViewLocation = glGetUniformLocation(shader.Program, "projectionView");
	glUniformMatrix4fv(projectionViewLocation, 1, GL_FALSE, glm::value_ptr(PV));
	GLint modelLocation = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

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
