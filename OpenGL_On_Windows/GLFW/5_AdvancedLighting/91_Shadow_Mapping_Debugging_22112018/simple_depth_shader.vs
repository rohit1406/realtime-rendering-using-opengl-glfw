#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 u_light_space_matrix;
uniform mat4 u_model;

void main()
{
	gl_Position = u_light_space_matrix * u_model * vec4(aPos, 1.0);
}