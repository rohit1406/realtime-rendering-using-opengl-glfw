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
//inverset the texture color
vec3 col = vec3(1.0 - texture(u_screen_texture, out_texcoords));
FragColor = vec4(col, 1.0);
}
