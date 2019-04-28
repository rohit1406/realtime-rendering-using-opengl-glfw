#version 330 core
//attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

//out parameters
out vec3 out_position;
out vec3 out_normal;

//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

void main()
{
//read multiplication from right to left
gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(aPos, 1.0);

out_normal = mat3(transpose(inverse(u_model_matrix))) * aNormal;
out_position = vec3(u_model_matrix * vec4(aPos, 1.0));
}