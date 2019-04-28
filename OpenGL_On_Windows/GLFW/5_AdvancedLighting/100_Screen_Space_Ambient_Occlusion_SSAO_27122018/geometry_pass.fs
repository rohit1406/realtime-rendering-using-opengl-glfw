#version 330 core

//in parameters
in vec3 out_frag_pos;
in vec3 out_normal;
in vec2 out_texcoords;

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
	gAlbedoSpecCBO.rgb = vec3(0.95);
}