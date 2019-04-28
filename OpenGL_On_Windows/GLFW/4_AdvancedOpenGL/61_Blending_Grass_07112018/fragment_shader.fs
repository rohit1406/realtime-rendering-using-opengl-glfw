#version 330 core

//in parameters
in vec2 out_texcoords;

//out parameters
out vec4 FragColor;

//uniforms
uniform sampler2D u_texture0;

//entry-point function
void main()
{
vec4 tex_color = texture(u_texture0, out_texcoords);

//if fragment gets the alpha value below certain threshold, discard it
//it only renders the fragments that are not almost completely transperent
if(tex_color.a < 0.1)
{
	discard;
}
FragColor = tex_color;
}
