#version 450 core

//attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexcoord;

//out parameters
out VS_OUT
{
 vec3 out_frag_pos;
 vec2 out_texcoords;
} vs_out;

//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

void main()
{
//read multiplication from right to left
gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(aPos, 1.0);

//calculate the vertex position in the world space coordinate system
vs_out.out_frag_pos = vec3(u_model_matrix * vec4(aPos, 1.0));

vs_out.out_texcoords = aTexcoord;
}