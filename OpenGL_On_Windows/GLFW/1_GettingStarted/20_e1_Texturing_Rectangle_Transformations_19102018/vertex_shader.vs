#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexcoord;
out vec3 out_color;
out vec2 out_texcoord;

uniform mat4 transform;

void main()
{
gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
out_color = aColor;
out_texcoord = aTexcoord;
}