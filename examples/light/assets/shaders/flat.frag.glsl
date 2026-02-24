#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 u_AlbedoColor;

void main()
{
	// Output fragment
    FragColor = vec4(u_AlbedoColor, 1.0);
}