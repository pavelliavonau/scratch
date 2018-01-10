#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "Mesh.h"
#include "TexturedBox.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Function prototypes
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// Window dimensions
static const GLuint WIDTH = 800, HEIGHT = 600;

// global transform
static glm::mat4 transform(1.0f);

int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

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
	glfwSetCursorPosCallback(window, cursor_position_callback);

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

	try
	{
		Scene scene;
		auto mesh = new Mesh({
				-1.0f , -0.5f, 0.0f, // Left
				 0.0f , -0.5f, 0.0f, // Right
				-0.5f ,  0.5f, 0.0f, // Top
				 0.0f ,  0.5f, 0.0f, // TopLeft
		   });
		mesh->blinking = true;
		scene.addObject(mesh);
		scene.addObject(new TexturedBox());

		while (!glfwWindowShouldClose(window))
		{
			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();

			// Render
			// Clear the colorbuffer
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glm::mat4 model(1.0f);
			model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			glm::mat4 view(1.0f);
			// Обратите внимание, что мы смещаем сцену в направлении обратном тому, в котором мы хотим переместиться
			view = ::transform * glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

			glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

			glm::mat4 PVM = projection * view * model;
			scene.render(PVM);

			// Swap the screen buffers
			glfwSwapBuffers(window);
		}
	} catch (...)
	{
		std::cout << "something catched :'(";
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/)
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
		::transform = glm::translate(transform, glm::vec3(-.1f, 0.0f, 0.0f));

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		::transform = glm::translate(transform, glm::vec3(.1f, 0.0f, 0.0f));

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		::transform = glm::translate(transform, glm::vec3(.0f, -.1f, 0.0f));

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		::transform = glm::translate(transform, glm::vec3(.0f, 0.1f, 0.0f));
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	static double x = 0;
	static double y = 0;

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
		::transform = glm::translate(transform, glm::vec3((x - xpos) * .1, (y - ypos) * .1, 0.0f));

	x = xpos;
	y = ypos;
}
