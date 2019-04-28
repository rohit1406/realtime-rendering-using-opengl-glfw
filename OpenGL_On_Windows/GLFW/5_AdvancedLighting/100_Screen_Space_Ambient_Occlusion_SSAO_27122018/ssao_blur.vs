#version 330 core
//attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

//out params
out vec2 out_texcoords;


void main()
{
//read multiplication from right to left
gl_Position = vec4(aPos, 1.0);
out_texcoords = aTexCoords;
}