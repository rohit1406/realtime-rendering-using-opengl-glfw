#version 330 core

uniform vec3 u_light_color;

layout (location = 0) out vec4 FragColor;

void main()
{           
    FragColor = vec4(u_light_color, 1.0);
}