#version 330 core

in vec2 out_texcoord;

out vec4 FragColor;

//texture sampler
uniform sampler2D u_sampler_swap;

void main()
{
FragColor = texture(u_sampler_swap, out_texcoord);
}