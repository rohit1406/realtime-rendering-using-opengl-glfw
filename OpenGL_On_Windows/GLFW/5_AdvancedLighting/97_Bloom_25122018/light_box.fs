#version 330 core

//input params
in VS_OUT
{
	vec3 out_frag_pos;
	vec3 out_normal;
	vec2 out_texcoords;
}fs_in;

//uniforms
uniform vec3 u_light_color;

//out params
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

//entry-point function
void main()
{
	FragColor = vec4(u_light_color, 1.0);
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 1.0)
	{
		BrightColor = vec4(FragColor.rgb, 1.0);
	}
}