#version 330 core

//attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexcoords;

//uniforms
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

//out params
out vec2 out_tex_coords;

void main()
{
	out_tex_coords = aTexcoords;
	
	mat4 rotView = mat4(mat3(u_view_matrix));
	vec4 clipPos = u_perspective_matrix * rotView * vec4(aPos, 1.0);

gl_Position =clipPos.xyww;
}