#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "Mesh.h"
#include "TexturedMesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Function prototypes
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

#define DEBUG_MESSAGES

#ifdef DEBUG_MESSAGES
void DebugMessageCallback(GLenum /*source*/,
                          GLenum type,
                          GLuint id,
                          GLenum severity,
                          GLsizei /*length*/,
                          const GLchar* message,
                          const void* userParam)
{
	std::ostream& stream = GL_DEBUG_TYPE_ERROR == type ? std::cerr : std::cout;
	stream << "---------------------opengl-debug-callback-start------------" << std::endl;
	stream << "message: "<< message << std::endl;
	stream << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		stream << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		stream << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		stream << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		stream << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		stream << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		stream << "OTHER";
		break;
	}
	stream << std::endl;

	stream << "id: " << id << std::endl;
	stream << "severity: ";
	switch (severity){
	case GL_DEBUG_SEVERITY_LOW:
		stream << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		stream << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		stream << "HIGH";
		break;
	}
	stream << std::endl;
	const std::string* userString = static_cast<const std::string*>(userParam);
	stream << *userString << std::endl;
	stream << "---------------------opengl-debug-callback-end--------------" << std::endl;
}

std::string debug_user_param;
#endif

// Error callback function prints out any errors from GFLW to the console
static void error_callback(int error, const char *description)
{
	std::cout << error << '\t' << description << std::endl;
}

// Window dimensions
static const GLuint WIDTH = 1024, HEIGHT = 768;

// Camera
static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
static GLfloat yaw   = -90.0f;
static GLfloat pitch = 0.0f;
static GLfloat fov = 45.0f;

static bool keys[1024];

static void do_movement(bool keys[])
{
	// Camera controls
	GLfloat cameraSpeed = 0.05f;

	if(keys[GLFW_KEY_LEFT_SHIFT])
		cameraSpeed *= 5;

	if(keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if(keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if(keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if(keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void scroll_callback(GLFWwindow* , double , double yoffset)
{
	if(fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if(fov <= 1.0f)
		fov = 1.0f;
	if(fov >= 45.0f)
		fov = 45.0f;
}

int main()
{
	glfwSetErrorCallback(error_callback);
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	if(GLFW_FALSE == glfwInit()){
		std::cout << "Failed to init GLFW" << std::endl;
		return -1;
	}
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#ifdef DEBUG_MESSAGES
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

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
	glfwSetScrollCallback(window, scroll_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

#ifdef DEBUG_MESSAGES
	if (GLEW_KHR_debug) {
		std::cout << "KHR_debug supported" << std::endl;
	}

	GLint v;
	glGetIntegerv( GL_CONTEXT_FLAGS, &v );
	if (v & GL_CONTEXT_FLAG_DEBUG_BIT) {
		std::cout << "OpenGL debug context present" << std::endl;
	}

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(static_cast<GLDEBUGPROC>(DebugMessageCallback), &debug_user_param);
#endif

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	{
		Scene scene;
		std::vector<GLfloat> cubeData = {
			//vertex            color                  UV
			-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		std::vector<glm::vec3> cubePositions = {
			{ 0.0f,  0.0f,  0.0f},
			{ 2.0f,  5.0f, -15.0f},
			{-1.5f, -2.2f, -2.5f},
			{-3.8f, -2.0f, -12.3f},
			{ 2.4f, -0.4f, -3.5f},
			{-1.7f,  3.0f, -7.5f},
			{ 1.3f, -2.0f, -2.5f},
			{ 1.5f,  2.0f, -2.5f},
			{ 1.5f,  0.2f, -1.5f},
			{-1.3f,  1.0f, -1.5f}
		};

		for(auto pos : cubePositions)
		{
			try
			{
				auto mesh = new TexturedMesh(cubeData);
				mesh->pos = pos;
				scene.addObject(mesh);
			} catch (const std::exception& e) {
				std::cout << e.what();
			}
		}

		try
		{
			while (!glfwWindowShouldClose(window))
			{
				// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
				glfwPollEvents();
				do_movement(keys);
				// Render
				// Clear the colorbuffer
				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glm::mat4 view = glm::lookAt(cameraPos,
				                             cameraPos + cameraFront,
				                             cameraUp);
				glm::mat4 projection = glm::perspective(glm::radians(fov), static_cast<float>(width) / height, 0.1f, 100.0f);
				glm::mat4 PV = projection * view;
				scene.render(PV);

				// Swap the screen buffers
				glfwSwapBuffers(window);
			}
		} catch (...)
		{
			std::cout << "something catched :'(";
		}
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

	if(action == GLFW_PRESS)
		keys[key] = true;
	else if(action == GLFW_RELEASE)
		keys[key] = false;
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	static double x = 0;
	static double y = 0;

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		double xoffset = xpos - x;
		double yoffset = y - ypos;

		double sensitivity = 0.1;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw   += xoffset;
		pitch += yoffset;

		if(pitch > 89.0f)
			pitch = 89.0f;
		if(pitch < -89.0f)
			pitch = -89.0f;

		glm::dvec3 front;
		front.x = cos(glm::radians(static_cast<double>(yaw))) * cos(glm::radians(static_cast<double>(pitch)));
		front.y = sin(glm::radians(static_cast<double>(pitch)));
		front.z = sin(glm::radians(static_cast<double>(yaw))) * cos(glm::radians(static_cast<double>(pitch)));
		cameraFront = glm::normalize(front);
	}

	x = xpos;
	y = ypos;
}
