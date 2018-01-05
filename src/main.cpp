#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SOIL.h>

#include "Shader.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
static const GLuint WIDTH = 800, HEIGHT = 600;

static float hOffset = 0.0;
static float vOffset = 0.0;

int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    glfwInit();
	// Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

	// Define the viewport dimensions
    int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader simpleShader("shaders/vertexSimple.glsl", "shaders/fragmentSimple.glsl");
	Shader colorShader ("shaders/vertexColor.glsl" , "shaders/fragmentColor.glsl");

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

	GLuint VBO1, VAO1;
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO1 as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO1 (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	GLuint VBO2, VAO2;//, EBO;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(GLfloat), &vertices[9], GL_STATIC_DRAW);

	// Vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), static_cast<GLvoid*>(0));
	glEnableVertexAttribArray(0);

	// Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO1 as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO1 (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat rect_vertices[] = {
		// Positions          // Colors           // Texture Coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.9f, 0.9f, // Top Right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.9f, 0.1f, // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.1f, 0.1f, // Bottom Left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.1f, 0.9f  // Top Left
	};
	GLuint rect_indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);

	// Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), static_cast<GLvoid*>(0));
	glEnableVertexAttribArray(0);
	// Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO


	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int imageWidth, imageHeight;
    unsigned char* image = SOIL_load_image("textures/container.jpg", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint texture1;
	glGenTextures(1, &texture1);

	glBindTexture(GL_TEXTURE_2D, texture1);

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//int imageWidth, imageHeight;
    unsigned char* image1 = SOIL_load_image("textures/awesomeface.png", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);

	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		 // Draw our first triangle
		simpleShader.Use();
		// Обновляем цвет формы
        GLdouble timeValue = glfwGetTime();
        GLdouble greenValue = (sin(timeValue) / 2) + 0.5;
		GLint vertexColorLocation = glGetUniformLocation(simpleShader.Program, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, static_cast<GLfloat>(greenValue), 0.0f, 1.0f);

		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		colorShader.Use();
		GLint hOffsetLocation = glGetUniformLocation(colorShader.Program, "hoffset");
		glUniform1f(hOffsetLocation, hOffset);
		GLint vOffsetLocation = glGetUniformLocation(colorShader.Program, "voffset");
		glUniform1f(vOffsetLocation, vOffset);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(colorShader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(colorShader.Program, "ourTexture2"), 1);
//		glBindVertexArray(VAO2);
//		glDrawArrays(GL_TRIANGLES, 0, 3);

//		glBindVertexArray(0);

		//colorShader.Use();
		// Draw container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);

	// Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
	{
		static bool wireframe = false;
		wireframe = !wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		::hOffset -= 0.1f;

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		::hOffset += 0.1f;

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		::vOffset -= 0.1f;

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		::vOffset += 0.1f;
}
