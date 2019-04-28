#version 330 core
//attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexcoord;

//out variables
out vec3 out_color;
out vec2 out_texcoord;

//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

void main()
{
//read multiplication from right to left
gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(aPos, 1.0);
out_color = aColor;
out_texcoord = aTexcoord;
}