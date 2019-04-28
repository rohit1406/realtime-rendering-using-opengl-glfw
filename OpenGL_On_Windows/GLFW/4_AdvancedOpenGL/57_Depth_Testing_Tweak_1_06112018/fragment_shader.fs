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
//FragColor = texture(u_texture0, out_texcoords);
FragColor = vec4(vec3(gl_FragCoord.z), 1.0);;
}
