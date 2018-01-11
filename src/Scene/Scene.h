#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include <glm/glm.hpp>

class SceneObject;

class Scene
{
public:
	~Scene();

	void init();
	void addObject(SceneObject*);
	void render(const glm::mat4& PV);

private:
	std::vector<SceneObject*> sceneObjects;
};

#endif // SCENE_H
