#version 330 core

in vec3 out_color;
in vec2 out_texcoord;
out vec4 FragColor;

uniform sampler2D u_texture_sampler_box;
uniform sampler2D u_texture_sampler_smiley;
uniform float opacity;
void main()
{
FragColor = mix(texture(u_texture_sampler_box, out_texcoord), texture(u_texture_sampler_smiley, out_texcoord), opacity);
}