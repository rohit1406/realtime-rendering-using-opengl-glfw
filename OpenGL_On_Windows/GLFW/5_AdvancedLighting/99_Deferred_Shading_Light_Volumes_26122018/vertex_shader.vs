#version 330 core

//attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

//out parameters
out vec3 out_frag_pos;
out vec3 out_normal;
out vec2 out_texcoords;

//entry-point function
void main()
{
	//read multiplication from right to left
	vec4 worldPos = u_model_matrix * vec4(aPos, 1.0);
	out_frag_pos = worldPos.xyz;

	out_texcoords = aTexcoords;
	
	mat3 normalMatrix = transpose(inverse(mat3(u_model_matrix)));
	out_normal = normalMatrix * aNormal;
	
	gl_Position = u_perspective_matrix * u_view_matrix * worldPos;
}