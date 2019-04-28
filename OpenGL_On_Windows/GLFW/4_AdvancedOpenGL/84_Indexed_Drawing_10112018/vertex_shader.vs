#version 330 core
//attributes
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

//out params
out vec3 out_color;

//uniforms
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;
uniform vec2 offsets[100];

void main()
{
vec2 offset = offsets[gl_InstanceID];
//read multiplication from right to left
gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(aPos +  offset, 0.0, 1.0);
out_color = aColor;
gl_PointSize = gl_Position.z;
}