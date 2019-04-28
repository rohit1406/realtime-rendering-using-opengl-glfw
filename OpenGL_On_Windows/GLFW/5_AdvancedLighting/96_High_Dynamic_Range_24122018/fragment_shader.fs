#version 330 core

//global variables
struct S_Light
{
	vec3 Position;
	vec3 Color;
};

//input params
in VS_OUT
{
	vec3 out_frag_pos;
	vec3 out_normal;
	vec2 out_texcoords;
}fs_in;

//uniforms
uniform vec3 u_viewer_position;
uniform S_Light lights[16];
uniform sampler2D u_sampler_diffuse_texture;

//out params
out vec4 FragColor;


//entry-point function
void main()
{
	vec3 color = texture(u_sampler_diffuse_texture, fs_in.out_texcoords).rgb;
	vec3 normal = normalize(fs_in.out_normal);
	
	//ambient
	vec3 ambient = 0.0 * color;
	
	//lighting
	vec3 lighting = vec3(0.0);
	for(int i=0; i<16; i++)
	{
		//diffuse
		vec3 lightDir = normalize(lights[i].Position - fs_in.out_frag_pos);
		float diff = max(dot(lightDir, normal), 0.0);
		vec3 diffuse = lights[i].Color * diff * color;
		vec3 result = diffuse;
		//attenuation
		//use quadratic as we have gamma correction
		float distance = length(fs_in.out_frag_pos - lights[i].Position);
		result *= 1.0 / (distance * distance);
		lighting += result;
	}
	FragColor = vec4(ambient + lighting, 1.0);
}