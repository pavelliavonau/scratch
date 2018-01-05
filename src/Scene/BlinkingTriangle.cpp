#include "BlinkingTriangle.h"
#include <cmath>

BlinkingTriangle::BlinkingTriangle()
    :shader("shaders/vertexSimple.glsl", "shaders/fragmentSimple.glsl")
{
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
         -1.0f , -0.5f, 0.0f, // Left
          0.0f , -0.5f, 0.0f, // Right
         -0.5f ,  0.5f, 0.0f, // Top
        // coordinates			colors				texture
         0.0f, -0.5f, 0.0f, 	1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Left
         1.0f, -0.5f, 0.0f, 	0.0f, 1.0f, 0.0f,	0.5f, 0.5f, // Right
         0.5f,  0.5f, 0.0f, 	0.0f, 0.0f, 1.0f,	0.5f, 1.0f, // Top
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO1 as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO1 (it's always a good thing to unbind any buffer/array to prevent strange bugs)

}

BlinkingTriangle::~BlinkingTriangle()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void BlinkingTriangle::render(double time)
{
    // Draw our first triangle
    shader.Use();
    // Обновляем цвет формы
    GLdouble greenValue = (sin(time) / 2) + 0.5;
    GLint vertexColorLocation = glGetUniformLocation(shader.Program, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, static_cast<GLfloat>(greenValue), 0.0f, 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
