#version 330 core

layout (location = 0) in vec4 aPosition;

out vec2 out_texcoords;

uniform mat4 u_model;
uniform mat4 u_projection;

void main()
{
	out_texcoords=aPosition.zw;
	gl_Position =u_projection * u_model * vec4(aPosition.xy, 0.0, 1.0);
	
}