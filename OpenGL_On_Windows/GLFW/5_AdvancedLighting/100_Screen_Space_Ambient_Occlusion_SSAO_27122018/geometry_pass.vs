#version 330 core

//attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoords;

//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;
uniform bool u_invert_normals;

//out parameters
out vec3 out_frag_pos;
out vec3 out_normal;
out vec2 out_texcoords;

//entry-point function
void main()
{
	//read multiplication from right to left
	vec4 viewPos = u_view_matrix * u_model_matrix * vec4(aPos, 1.0);
	out_frag_pos = viewPos.xyz;

	out_texcoords = aTexcoords;
	
	mat3 normalMatrix = transpose(inverse(mat3(u_view_matrix * u_model_matrix)));
	out_normal = normalMatrix * (u_invert_normals ? -aNormal : aNormal);
	
	gl_Position = u_perspective_matrix * viewPos;
}