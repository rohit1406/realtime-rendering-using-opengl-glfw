#version 330 core
//attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexcoords;

//out parameters
out vec2 out_texcoords;

//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

void main()
{
//read multiplication from right to left
//gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(aPos, 1.0);

gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
out_texcoords = aTexcoords;
}