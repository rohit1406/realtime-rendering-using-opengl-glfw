#version 450 core
//attributes
layout (location = 0) in vec4 aPos;

//out parameters
out vec2 out_texcoords;

//uniforms
uniform mat4 u_projection_matrix;

void main()
{
//read multiplication from right to left
gl_Position = u_projection_matrix * vec4(aPos.x, aPos.y, 0.0, 1.0);

out_texcoords = aPos.zw;
}