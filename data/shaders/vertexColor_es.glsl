#version 100

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

varying vec3 ourColor;
varying vec2 TexCoord;
varying vec3 Normal;

varying vec3 FragPos;

uniform mat4 projectionView;
uniform mat4 model;

void main()
{
        gl_Position = projectionView * model * vec4(position, 1.0);
        ourColor = vec3(1.0, 1.0, 1.0);
	TexCoord = texCoord;
        FragPos = vec3(model * vec4(position, 1.0));
        // mat3 normalMatrix = mat3(transpose(inverse(model))); // too expensive
        Normal = normal;//normalMatrix * normal;
}
