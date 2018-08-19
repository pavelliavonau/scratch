#version 100

attribute vec3 position;

uniform mat4 PVM;

void main()
{
        gl_Position =  PVM * vec4(position, 1.0);
}
