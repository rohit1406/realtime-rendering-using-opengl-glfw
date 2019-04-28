#version 450 core

//attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

//out parameters
out VS_OUT
{
 vec3 out_frag_pos;
 vec2 out_texcoords;
 vec3 out_tangent_light_pos;
 vec3 out_tangent_view_pos;
 vec3 out_tangent_frag_pos;
} vs_out;

//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;
uniform vec3 u_light_pos;
uniform vec3 u_viewer_pos;

void main()
{
//read multiplication from right to left
gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(aPos, 1.0);

//calculate the vertex position in the world space coordinate system
vs_out.out_frag_pos = vec3(u_model_matrix * vec4(aPos, 1.0));

vs_out.out_texcoords = aTexcoord;

	mat3 normalMatrix = transpose(inverse(mat3(u_model_matrix)));
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 N = normalize(normalMatrix * aNormal);
	// re-orthogonalize T with respect to N - Gram-Schmidt process
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	
	mat3 TBN = transpose(mat3(T, B, N));
	vs_out.out_tangent_light_pos = TBN * u_light_pos;
	vs_out.out_tangent_view_pos = TBN * u_viewer_pos;
	vs_out.out_tangent_frag_pos = TBN * vs_out.out_frag_pos;
}