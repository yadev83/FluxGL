#version 460 core
out vec4 FragColor;

uniform vec3 u_AlbedoColor;

void main() {
    FragColor = vec4(u_AlbedoColor, 1.0);
}