#version 330 core

layout (location = 0) in vec4 aPosition;

out vec2 out_texcoords;
out vec4 out_particle_color;

uniform mat4 u_projection;
uniform vec2 u_offset;
uniform vec4 u_color;

void main()
{
	float scale = 10.0f;
	out_texcoords = aPosition.zw;
	out_particle_color = u_color;
	gl_Position = u_projection * vec4((aPosition.xy * scale) + u_offset, 0.0, 1.0);
}