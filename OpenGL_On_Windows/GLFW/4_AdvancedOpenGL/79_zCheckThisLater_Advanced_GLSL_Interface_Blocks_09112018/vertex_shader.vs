#version 330 core
//attributes
layout (location = 0) in vec3 aPos;


//uniforms
layout (std140) uniform u_transformation_matrices
{
	mat4 u_view_matrix;
	mat4 u_perspective_matrix;
};
uniform mat4 u_model_matrix;


void main()
{
//read multiplication from right to left
gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(aPos, 1.0);

}