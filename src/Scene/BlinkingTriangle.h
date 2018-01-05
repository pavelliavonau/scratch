#ifndef BLINKINGTRIANGLE_H
#define BLINKINGTRIANGLE_H

#include "SceneObject.h"
#include <GL/glew.h>
#include <Shader.h>

class BlinkingTriangle : public SceneObject
{
public:
    BlinkingTriangle();
    ~BlinkingTriangle();

    void render(double time) override;

private:
    GLuint VBO, VAO;
    Shader shader;
};

#endif // BLINKINGTRIANGLE_H
