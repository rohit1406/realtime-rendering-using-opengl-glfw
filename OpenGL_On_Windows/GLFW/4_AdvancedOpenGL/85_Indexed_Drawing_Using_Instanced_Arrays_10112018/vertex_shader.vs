#version 330 core
//attributes
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

//out params
out vec3 out_color;

//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

void main()
{
//downscale each quad
vec2 pos = aPos * (gl_InstanceID / 100.0);

//read multiplication from right to left
gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(pos +  aOffset, 0.0, 1.0);
out_color = aColor;
gl_PointSize = gl_Position.z;
}