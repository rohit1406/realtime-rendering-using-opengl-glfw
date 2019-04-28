#version 330 core

//in params
in  vec3 out_local_pos;

//uniforms
//materials
uniform samplerCube u_sampler_environment_map;

//out params
out vec4 FragColor;

//entry-point function
void main()
{
	//vec3 envColor = texture(u_sampler_environment_map, out_local_pos).rgb;
	vec3 envColor = textureLod(u_sampler_environment_map, out_local_pos, 1.2).rgb;
	envColor = envColor / (envColor + vec3(1.0));
	envColor = pow(envColor, vec3(1.0/2.2));
	FragColor = vec4(envColor, 1.0);
}

