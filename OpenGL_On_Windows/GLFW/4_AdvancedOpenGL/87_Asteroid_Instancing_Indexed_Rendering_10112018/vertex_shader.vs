#version 330 core
//attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in mat4 aInstanceMatrix;

//out parameters
out vec2 out_texcoords;

//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

void main()
{
//read multiplication from right to left
gl_Position = u_perspective_matrix * u_view_matrix * aInstanceMatrix * vec4(aPos, 1.0);

out_texcoords = aTexcoords;
}