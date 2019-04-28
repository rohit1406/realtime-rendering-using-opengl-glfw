#version 330 core
//attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

//out parameters
out VS_OUT {
    vec3 normal;
} vs_out;

//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

void main()
{
//read multiplication from right to left
gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(aPos, 1.0);

mat3 normalMatrix = mat3(transpose(inverse(u_view_matrix * u_model_matrix)));
vs_out.normal = normalize(vec3(u_perspective_matrix * vec4(normalMatrix * aNormal, 0.0)));
}