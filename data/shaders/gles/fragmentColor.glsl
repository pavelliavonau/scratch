#version 100

precision mediump float;

varying vec3 ourColor;
varying vec2 TexCoord;
varying vec3 Normal;

varying vec3 FragPos;

uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

uniform float useTextureRate;

void main()
{
        vec3 objectColor = mix(texture2D(ourTexture1, vec2(TexCoord.x, 1.0 - TexCoord.y)).xyz, ourColor, useTextureRate);

	// redo hardcode
        float ambientStrength = 0.1;
        vec3 lightColor = vec3(1.0, 1.0, 1.0);
        vec3 lightPos = vec3(1.2, 1.0, 2.0);

	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * objectColor;
        gl_FragColor = vec4(result, 1.0);
}
