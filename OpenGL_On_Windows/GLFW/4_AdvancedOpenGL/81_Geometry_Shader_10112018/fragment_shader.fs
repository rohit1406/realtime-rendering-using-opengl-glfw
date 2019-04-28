#version 450 core

//in params
in vec3 out_fColor; 

//out parameters
out vec4 FragColor;

//entry-point function
void main()
{
FragColor = vec4(out_fColor, 1.0);;
}
