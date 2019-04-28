#version 330 core

//in attributes
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexcoord;

//uniforms
//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

//out params
out vec2 out_texcoord;
void main()
{
gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(aPos, 0.0, 1.0);
out_texcoord = vec2(aTexcoord.x, -aTexcoord.y);
}