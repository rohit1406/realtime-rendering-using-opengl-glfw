#version 330 core
//attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

//out parameters
out vec3 out_normals;
out vec3 out_frag_pos;

//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

void main()
{
//read multiplication from right to left
gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(aPos, 1.0);

//calculate the vertex position in the world space coordinate system
out_frag_pos = vec3(u_model_matrix * vec4(aPos, 1.0));

//calculate the normal matrix as we are doing calculations on the vertex pos in world coordinate space
//this can be calculated by doing transpose of inlverse of upper left corner of the model matrix
//we are using mat3 because it removes the translation portion of the model matrix as normals don't have homogeneous coordinate component w
out_normals = mat3(transpose(inverse(u_model_matrix))) * aNormal;
}