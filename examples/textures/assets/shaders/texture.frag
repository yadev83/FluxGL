#version 330 core
out vec4 FragColor;

in vec3 Color;
in vec2 TexCoord;

uniform vec3 u_AlbedoColor;

// texture sampler
#define MAX_ALBEDO_TEXTURES 4
uniform sampler2D u_Albedo[MAX_ALBEDO_TEXTURES];
uniform int u_AlbedoTextureCount;

void main()
{
	vec4 finalColor = vec4(u_AlbedoColor, 1.0);

	for(int i = 0; i < MAX_ALBEDO_TEXTURES && i < u_AlbedoTextureCount; i++) {
		vec4 texColor = texture(u_Albedo[i], TexCoord);

		finalColor = mix(finalColor, texColor, texColor.a);
	}

    FragColor = finalColor;
}