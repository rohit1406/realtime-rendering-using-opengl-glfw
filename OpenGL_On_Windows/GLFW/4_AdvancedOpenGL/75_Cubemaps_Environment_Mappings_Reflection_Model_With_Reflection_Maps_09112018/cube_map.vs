#version 330 core
//attributes
layout (location = 0) in vec3 aPos;

//out parameters
out vec3 out_texcoords;

//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

void main()
{
//read multiplication from right to left
vec4 pos =  u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(aPos, 1.0);
gl_Position = pos.xyww; //to make z component always 1 after perspective division
out_texcoords = aPos;
}