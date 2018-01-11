#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

uniform float useTextureRate;

void main()
{
	color = mix(texture(ourTexture1, vec2(TexCoord.x, 1.0 - TexCoord.y)), vec4(ourColor, 1.0f), useTextureRate);
}
