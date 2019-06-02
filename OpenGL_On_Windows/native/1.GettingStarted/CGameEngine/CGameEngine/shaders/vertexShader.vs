#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec3 out_Color;

uniform float u_position_offset;

void main()
{
    gl_Position = vec4(aPosition.x + u_position_offset, aPosition.y, aPosition.z, 1.0);
    out_Color = aColor;
}