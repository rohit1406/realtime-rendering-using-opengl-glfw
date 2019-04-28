#version 330 core

//in parameters
in vec3 out_texcoords;

//out parameters
out vec4 FragColor;

//uniforms
uniform samplerCube u_skybox_cubemap_sampler;

//entry-point function
void main()
{
FragColor = texture(u_skybox_cubemap_sampler, out_texcoords);
}
