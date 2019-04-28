#version 330 core
//attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;
uniform bool u_inverse_normals;

//out params
out VS_OUT
{
	vec3 out_frag_pos;
	vec3 out_normal;
	vec2 out_texcoords;
}vs_out;

void main()
{
//read multiplication from right to left
gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(aPos, 1.0);

vs_out.out_frag_pos = vec3(u_model_matrix * vec4(aPos, 1.0));

vs_out.out_texcoords = aTexCoords;

vec3 n = u_inverse_normals ? -aNormal : aNormal;
mat3 normal_matrix = transpose(inverse(mat3(u_model_matrix)));
vs_out.out_normal = normalize(normal_matrix * n);
}