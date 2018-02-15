#version 330 core

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;

in vec3 FragPos;

out vec4 color;

uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

uniform float useTextureRate;

void main()
{
	vec3 objectColor = mix(texture(ourTexture1, vec2(TexCoord.x, 1.0 - TexCoord.y)).xyz, ourColor, useTextureRate);

	// redo hardcode
	float ambientStrength = 0.1f;
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);

	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * objectColor;
	color = vec4(result, 1.0f);
}
