#include "Camera.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

Camera &Camera::instance()
{
	static Camera camera;
	return camera;
}

void Camera::init(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
{
	this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraPos = position;
	this->cameraUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->updateVectors();
}

void Camera::doMovement(bool keys[])
{
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

void Camera::doRotation(double dx, double dy)
{
	double sensitivity = 0.1;
	dx *= sensitivity;
	dy *= sensitivity;

	yaw   += dx;
	pitch += dy;

	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	updateVectors();
}

void Camera::doFov(double dy)
{
	if(fov >= 1.0f && fov <= 45.0f)
		fov -= dy;
	if(fov <= 1.0f)
		fov = 1.0f;
	if(fov >= 45.0f)
		fov = 45.0f;
}

glm::mat4 Camera::view() const
{
	return glm::lookAt(cameraPos,
	                   cameraPos + cameraFront,
	                   cameraUp);
}

GLfloat Camera::getFov() const
{
	return fov;
}

void Camera::updateVectors()
{
	glm::dvec3 front;
	front.x = cos(glm::radians(static_cast<double>(yaw))) * cos(glm::radians(static_cast<double>(pitch)));
	front.y = sin(glm::radians(static_cast<double>(pitch)));
	front.z = sin(glm::radians(static_cast<double>(yaw))) * cos(glm::radians(static_cast<double>(pitch)));
	cameraFront = glm::normalize(front);
}
