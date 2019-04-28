#version 330 core

//in parameters
in vec2 out_texcoords;

//out parameters
out vec4 FragColor;

//uniforms
uniform sampler2D u_screen_texture;

//entry-point function
void main()
{
vec3 col = texture(u_screen_texture, out_texcoords).rgb;
FragColor = vec4(col, 1.0);
}
