#version 330 core

//in parameters
in vec3 out_frag_pos;
in vec3 out_normal;
in vec2 out_texcoords;


//uniforms
uniform sampler2D u_sampler_texture_diffuse1;
uniform sampler2D u_sampler_texture_sepcular1;

//out parameters
layout (location = 0) out vec3 gPositionCBO;
layout (location = 1) out vec3 gNormalCBO;
layout (location = 2) out vec4 gAlbedoSpecCBO;

void main()
{
	//store the fragment position vector in the first gbuffer texture
	gPositionCBO = out_frag_pos;
	//store the per-fragment normals into the gbuffer
	gNormalCBO = normalize(out_normal);
	//and diffuse per-fragment color
	gAlbedoSpecCBO.rgb = texture(u_sampler_texture_diffuse1, out_texcoords).rgb;
	//store specular intensity in alpha component
	gAlbedoSpecCBO.a = texture(u_sampler_texture_sepcular1, out_texcoords).r;
}