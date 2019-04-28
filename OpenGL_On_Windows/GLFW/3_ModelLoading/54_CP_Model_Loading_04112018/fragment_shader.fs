#version 330 core

//in parameters
in vec2 out_texcoords;

//out parameters
out vec4 FragColor;

//uniforms
uniform sampler2D texture_diffuse1;

void main()
{
FragColor = texture(texture_diffuse1, out_texcoords);
}