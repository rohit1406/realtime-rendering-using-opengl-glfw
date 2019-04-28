#version 330 core

//in attributes
in vec2 out_texcoords;

//uniforms
uniform sampler2D u_sampler_hdr_color;
uniform bool u_hdr;
uniform float u_exposure;

//out params
out vec4 FragColor;

void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(u_sampler_hdr_color, out_texcoords).rgb;
	
	if(u_hdr)
	{
		//exposure
		vec3 result = vec3(1.0) - exp(-hdrColor * u_exposure);
		
		//gamma correct
		result = pow(result, vec3(1.0 / gamma));
		
		FragColor = vec4(result, 1.0);
	}else
	{
		vec3 result = pow(hdrColor, vec3(1.0 / gamma));
		FragColor = vec4(result, 1.0);
	}
}