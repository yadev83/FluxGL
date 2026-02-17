#version 460 core
out vec4 FragColor;

uniform vec3 u_AlbedoColor;
uniform vec3 u_LightColor;

void main() {
    FragColor = vec4(u_LightColor * u_AlbedoColor, 1.0);
}