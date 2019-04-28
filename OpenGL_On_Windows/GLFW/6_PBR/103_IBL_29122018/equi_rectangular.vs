#version 330 core

//attributes
layout (location = 0) in vec3 aPos;

//uniforms
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

//out params
out vec3 out_local_pos;

void main()
{
	out_local_pos = aPos;
	
//read multiplication from right to left
gl_Position = u_perspective_matrix * u_view_matrix * vec4(out_local_pos, 1.0);
}