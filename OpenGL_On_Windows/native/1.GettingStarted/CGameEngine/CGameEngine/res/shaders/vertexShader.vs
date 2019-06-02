#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

out vec3 out_Color;
out vec2 out_texcoord;

uniform float u_position_offset;

void main()
{
    gl_Position = vec4(aPosition.x + u_position_offset, aPosition.y, aPosition.z, 1.0);
    out_Color = aColor;
	out_texcoord = aTexCoords;
}