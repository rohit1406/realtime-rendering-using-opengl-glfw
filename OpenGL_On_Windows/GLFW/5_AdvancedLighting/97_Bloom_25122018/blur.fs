#version 330 core

//in attributes
in vec2 out_texcoords;

//uniforms
uniform sampler2D u_sampler_image;
uniform bool u_horizontal;
uniform float u_weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

//out params
out vec4 FragColor;

void main()
{
	vec2 tex_offset = 1.0 / textureSize(u_sampler_image, 0); //get size of a single texel
	vec3 result = texture(u_sampler_image, out_texcoords).rgb * u_weight[0];
	
	if(u_horizontal)
	{
		for(int i=0; i<5; i++)
		{
			result += texture(u_sampler_image, out_texcoords + vec2(tex_offset.x * i, 0.0)).rgb * u_weight[i];
			result += texture(u_sampler_image, out_texcoords - vec2(tex_offset.x * i, 0.0)).rgb * u_weight[i];
		}
	}
	else
	{
		for(int i=0; i<5; i++)
		{
			result += texture(u_sampler_image, out_texcoords + vec2(0.0, tex_offset.y * i)).rgb * u_weight[i];
			result += texture(u_sampler_image, out_texcoords - vec2(0.0, tex_offset.y * i)).rgb * u_weight[i];
		}
	}
	FragColor = vec4(result, 1.0);
}