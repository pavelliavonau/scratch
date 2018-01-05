#ifndef SCENE_H
#define SCENE_H

#include <vector>

class SceneObject;

class Scene
{
public:
    ~Scene();

    void init();
    void addObject(SceneObject*);
    void render();

private:
    std::vector<SceneObject*> sceneObjects;
};

#endif // SCENE_H
