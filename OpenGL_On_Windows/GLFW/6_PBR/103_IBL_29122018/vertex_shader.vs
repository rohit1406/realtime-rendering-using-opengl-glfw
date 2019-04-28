#version 330 core

//attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormals;

//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

//out params
out vec2 out_texcoords;
out vec3 out_world_pos;
out vec3 out_normals;

void main()
{
	out_texcoords = aTexCoords;
	out_world_pos = vec3(u_model_matrix * vec4(aPos, 1.0));
	out_normals = mat3(u_model_matrix) * aNormals;
	
//read multiplication from right to left
gl_Position = u_perspective_matrix * u_view_matrix * vec4(out_world_pos, 1.0);
}