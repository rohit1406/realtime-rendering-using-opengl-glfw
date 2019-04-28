#version 330 core

in vec3 out_color;
in vec2 out_texcoord;
out vec4 FragColor;

//texture sampler
uniform sampler2D u_texture_sampler;

void main()
{
FragColor = texture(u_texture_sampler, out_texcoord) * vec4(out_color, 1.0);
}