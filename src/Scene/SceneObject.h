#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <glm/glm.hpp>

class SceneObject
{
public:
	//SceneObject();
	virtual ~SceneObject() {}

	virtual void render(double time, const glm::mat4& PV) = 0;
};

#endif // SCENEOBJECT_H
