#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	//color = vec4(ourColor, 1.0f);
	color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, vec2(/*1.0 -*/ TexCoord.x * 2, 1.0 - TexCoord.y * 2)), 0.3) * vec4(ourColor, 1.0f);
}
