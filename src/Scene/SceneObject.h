#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

class SceneObject
{
public:
	//SceneObject();
	virtual ~SceneObject() {}

	virtual void render(double time) = 0;
};

#endif // SCENEOBJECT_H
