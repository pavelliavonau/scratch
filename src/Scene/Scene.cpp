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

void Scene::render(const glm::mat4& PV)
{
	auto time = glfwGetTime();
	for(auto o : this->sceneObjects)
		o->render(time, PV);
}
