#version 330 core

in vec2 out_texcoords;
in vec4 out_particle_color;

out vec4 FragColor;

uniform sampler2D u_sampler_image;

void main()
{
	vec4 color = texture(u_sampler_image, out_texcoords);
	
	FragColor = color * out_particle_color; 
}