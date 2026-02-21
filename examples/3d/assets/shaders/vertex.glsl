#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec2 aUV;

out vec2 TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
	TexCoord = aUV;
}