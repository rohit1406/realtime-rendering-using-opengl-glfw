#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 u_model_matrix;

void main()
{
	gl_Position = u_model_matrix * vec4(aPos, 1.0);
}