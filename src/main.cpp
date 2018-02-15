#include <Light.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Scene.h"
#include "Mesh.h"
#include "TexturedMesh.h"
#include "ImportObj.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Function prototypes
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

#define DEBUG_MESSAGES

#ifdef DEBUG_MESSAGES
#include "UserDebugInfo.h"

void DebugMessageCallback(GLenum source,
                          GLenum type,
                          GLuint id,
                          GLenum severity,
                          GLsizei /*length*/,
                          const GLchar* message,
                          const void* userParam)
{
	std::ostream& stream = GL_DEBUG_TYPE_ERROR == type ? std::cerr : std::cout;
	stream << std::endl << "---------------------opengl-debug-callback-start------------" << std::endl;

	switch (severity){
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		stream << "NOTIFICATION";
		break;
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
	stream << "::";

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
	case GL_DEBUG_TYPE_MARKER:
		stream << "MARKER";
		break;
	case GL_DEBUG_TYPE_OTHER:
		stream << "OTHER";
		break;
	}
	stream << "::";

	switch (source){
	case GL_DEBUG_SOURCE_API:
		stream << "API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		stream << "WINDOW_SYSTEM";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		stream << "SHADER_COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		stream << "THIRD_PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		stream << "APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		stream << "OTHER";
		break;
	}
	stream << std::endl;

	stream << "id: " << id << std::endl;

	stream << "message: "<< std::endl <<  message << std::endl;

	const std::string* userString = static_cast<const std::string*>(userParam);
	stream << "user info: " << std::endl << *userString << std::endl;
	stream << "---------------------opengl-debug-callback-end--------------" << std::endl;
}
#endif

// Error callback function prints out any errors from GFLW to the console
static void error_callback(int error, const char *description)
{
	std::cout << error << '\t' << description << std::endl;
}

// Window dimensions
static const GLuint WIDTH = 1024, HEIGHT = 768;

static bool keys[1024];

void scroll_callback(GLFWwindow* , double , double dy)
{
	Camera::instance().doFov(dy);
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
	glDebugMessageCallback(static_cast<GLDEBUGPROC>(DebugMessageCallback), &UserDebugInfo::userInfoRef());
#endif

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	{
		Scene scene;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		{
			OBJLoader lo;
			lo.loadVerticesUVNormal("models/cube.obj", vertices, uvs, normals);
		}
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

		try
		{
			for(auto pos : cubePositions)
			{
				auto mesh = new TexturedMesh(vertices, uvs, normals);
				mesh->pos = pos * 2.0f;
				scene.addObject(mesh);
			}

			scene.addObject(new Light());

		} catch (const std::exception& e) {
			std::cout << e.what();
		}

		try
		{
			while (!glfwWindowShouldClose(window))
			{
				static float deltaTime = 0.0f;	// Время, прошедшее между последним и текущим кадром
				static float lastFrame = 0.0f;	// Время вывода последнего кадра

				float currentFrameTime = static_cast<float>(glfwGetTime());
				deltaTime = currentFrameTime - lastFrame;
				lastFrame = currentFrameTime;
				// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
				glfwPollEvents();
				Camera::instance().doMovement(keys, deltaTime);
				// Render
				// Clear the colorbuffer
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glm::mat4 view = Camera::instance().view();
				glm::mat4 projection = glm::perspective(glm::radians(Camera::instance().getFov()), static_cast<float>(width) / height, 0.1f, 100.0f);
				glm::mat4 PV = projection * view;
				scene.render(PV, currentFrameTime);

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

	double dx = xpos - x;
	double dy = y - ypos;

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		Camera::instance().doRotation(dx, dy);
	}

	x = xpos;
	y = ypos;
}
