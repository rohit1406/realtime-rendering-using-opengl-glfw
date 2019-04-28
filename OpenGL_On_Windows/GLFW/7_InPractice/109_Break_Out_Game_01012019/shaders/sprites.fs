#version 330 core

in vec2 out_texcoords;

out vec4 FragColor;

uniform sampler2D u_sampler_image;
uniform vec3 u_sprite_color;

void main()
{
	vec4 color = texture(u_sampler_image, out_texcoords);
	
	FragColor = vec4(u_sprite_color, 1.0) * color; 
}