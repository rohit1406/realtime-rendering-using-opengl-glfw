#version 330 core

//global variables
struct Light
{
	vec3 Position;
	vec3 Color;
	float Linear;
	float Quadratic;
	float Radius;
};
const int NR_LIGHTS = 32;

//in attributes
in vec2 out_texcoords;

//uniforms
uniform sampler2D u_sampler_gPositionCBO;
uniform sampler2D u_sampler_gNormalCBO;
uniform sampler2D u_sampler_gAlbedoSpecCBO;

uniform Light u_lights[NR_LIGHTS];
uniform vec3 u_viewer_position;

//out params
out vec4 FragColor;

void main()
{
	//retrieve data from gbuffer
	vec3 FragPos = texture(u_sampler_gPositionCBO, out_texcoords).rgb;
	vec3 Normal = texture(u_sampler_gNormalCBO, out_texcoords).rgb;
	vec3 Diffuse = texture(u_sampler_gAlbedoSpecCBO, out_texcoords).rgb;
	float Specular = texture(u_sampler_gAlbedoSpecCBO, out_texcoords).a;
	
	//calculate lighting as usual
	vec3 lighting = Diffuse * 0.1; //hard-coded ambient component
	vec3 viewDir = normalize(u_viewer_position - FragPos);
	for(int i=0; i<NR_LIGHTS; i++)
	{
		
		float distance = length(u_lights[i].Position - FragPos);
		if(distance < u_lights[i].Radius)
		{
		//diffuse
		vec3 lightDir = normalize(u_lights[i].Position - FragPos);
		vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * u_lights[i].Color;
		
		//specular
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(Normal, halfwayDir),0.0), 16.0);
		vec3 specular = u_lights[i].Color * spec * Specular;
		
		//attenuation
		float attenuation = 1.0 / (1.0 + u_lights[i].Linear * distance + u_lights[i].Quadratic * distance * distance);
		
		diffuse *= attenuation;
		specular *= attenuation;
		
		lighting += diffuse + specular;
		}
	}
	FragColor = vec4(lighting, 1.0);
}