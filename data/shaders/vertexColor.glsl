#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec3 ourColor;
out vec2 TexCoord;
out vec3 Normal;

out vec3 FragPos;

uniform mat4 projectionView;
uniform mat4 model;

void main()
{
	gl_Position = projectionView * model * vec4(position, 1.0f);
	ourColor = vec3(1.0f, 1.0f, 1.0f);
	TexCoord = texCoord;
	FragPos = vec3(model * vec4(position, 1.0f));
	mat3 normalMatrix = mat3(transpose(inverse(model))); // too expensive
	Normal = normalMatrix * normal;
}
