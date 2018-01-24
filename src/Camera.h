#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <GL/glew.h>

class Camera
{
public:
	static Camera& instance();

	void init(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = -90.0f, GLfloat pitch = 0.0f);

	void doMovement(bool keys[], float deltaTime);
	void doRotation(double dx, double dy);
	void doFov(double dy);

	glm::mat4 view() const;
	GLfloat getFov() const;

private:

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	GLfloat yaw   = -90.0f;
	GLfloat pitch = 0.0f;

	GLfloat fov = 45.0f;

	Camera() {}
	void updateVectors();
};

#endif// CAMERA_H
