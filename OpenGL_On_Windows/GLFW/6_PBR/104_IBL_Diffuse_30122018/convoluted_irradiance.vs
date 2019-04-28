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
	
	mat4 rotView = mat4(mat3(u_view_matrix));
	vec4 clipPos = u_perspective_matrix * rotView * vec4(out_local_pos, 1.0);

gl_Position =clipPos.xyww;
}