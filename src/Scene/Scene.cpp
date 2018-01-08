#include "Scene.h"
#include "SceneObject.h"
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

Scene::~Scene()
{
	for(auto o : this->sceneObjects)
		delete o;
}

void Scene::addObject(SceneObject * o)
{
	sceneObjects.push_back(o);
}

void Scene::render(const glm::mat4& transform)
{
	auto time = glfwGetTime();

	glm::mat4 trans = transform;
	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	trans = glm::rotate(trans, static_cast<GLfloat>(time) * 5.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	for(auto o : this->sceneObjects)
		o->render(time, trans);
}
