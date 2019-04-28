#version 330 core

out vec4 FragColor;

uniform vec3 u_light_color_lp;

void main()
{
FragColor = vec4(u_light_color_lp,1.0);
}