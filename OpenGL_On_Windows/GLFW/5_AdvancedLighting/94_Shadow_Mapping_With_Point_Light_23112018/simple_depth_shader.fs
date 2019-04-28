#version 330 core

in vec4 FragPos;

uniform vec3 u_light_pos;
uniform float u_far_plane;

void main()
{
	//get distance between fragment and light source
	float light_distance = length(FragPos.xyz - u_light_pos);
	
	//map to 0,1 range
	light_distance = light_distance / u_far_plane;
	
	//write this as a modified depth
	gl_FragDepth = light_distance;
}