#version 330 core

//in parameters
in vec2 out_texcoords;

//out parameters
out vec4 FragColor;

//uniforms
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;

//entry-point function
void main()
{
if(gl_FrontFacing)
FragColor = texture(u_texture0, out_texcoords);
else
FragColor = texture(u_texture1, out_texcoords);
}
