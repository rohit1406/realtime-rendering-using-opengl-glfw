#version 330 core

out vec4 FragColor;

//uniforms
uniform vec3 u_object_color;
uniform vec3 u_light_color;

void main()
{
FragColor = vec4(u_object_color * u_light_color,1.0);
}