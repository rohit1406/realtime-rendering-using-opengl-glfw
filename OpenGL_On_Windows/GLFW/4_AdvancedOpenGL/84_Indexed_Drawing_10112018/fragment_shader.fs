#version 330 core

//in params
in vec3 out_color;

//out parameters
out vec4 FragColor;

//entry-point function
void main()
{
FragColor = vec4(out_color, 1.0);;
}
