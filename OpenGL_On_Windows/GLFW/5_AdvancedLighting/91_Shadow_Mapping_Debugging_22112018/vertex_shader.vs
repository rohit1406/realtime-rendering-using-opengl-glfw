#version 450 core

//attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;

//out parameters
out VS_OUT
{
 vec3 out_normals;
 vec3 out_frag_pos;
 vec2 out_texcoords;
} vs_out;

//uniforms
uniform mat4 u_model;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

void main()
{
//read multiplication from right to left
gl_Position = u_perspective_matrix * u_view_matrix * u_model * vec4(aPos, 1.0);

//calculate the vertex position in the world space coordinate system
vs_out.out_frag_pos = vec3(u_model * vec4(aPos, 1.0));

//calculate the normal matrix as we are doing calculations on the vertex pos in world coordinate space
//this can be calculated by doing transpose of inlverse of upper left corner of the model matrix
//we are using mat3 because it removes the translation portion of the model matrix as normals don't have homogeneous coordinate component w
vs_out.out_normals = mat3(transpose(inverse(u_model))) * aNormal;

vs_out.out_texcoords = aTexcoord;
}