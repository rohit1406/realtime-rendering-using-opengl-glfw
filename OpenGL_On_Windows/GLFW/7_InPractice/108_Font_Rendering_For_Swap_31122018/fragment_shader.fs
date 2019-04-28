#version 450 core

//in parameters
in vec2 out_texcoords;

//out parameters
out vec4 FragColor;

//uniforms
uniform sampler2D u_sampler_text;
uniform vec3 u_text_color;

void main()
{
vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_sampler_text, out_texcoords).r);
FragColor = vec4(u_text_color, 1.0) * sampled;;
}