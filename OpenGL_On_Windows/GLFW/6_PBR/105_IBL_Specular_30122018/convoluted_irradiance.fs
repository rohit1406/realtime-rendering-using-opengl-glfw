#version 330 core

//global variables
const float PI = 3.14159265359;

//in params
in  vec3 out_local_pos;

//uniforms
//materials
uniform samplerCube u_sampler_convoluted_environment_map;

//out params
out vec4 FragColor;

//entry-point function
void main()
{
	vec3 normal = normalize(out_local_pos);
	vec3 irradiance = vec3(0.0);
	
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(up, normal);
	up = cross(normal, right);
	
	float sampleDelta = 0.025;
	float nrSamples = 0.0;
	for(float phi=0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
		for(float theta=0.0; theta<0.5*PI; theta+=sampleDelta)
		{
			//spherical to cartesian
			vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			//tanget to world space
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
			
			irradiance += texture(u_sampler_convoluted_environment_map, sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples++;
		}
	}
	irradiance = PI * irradiance * (1.0/float(nrSamples));
	
	FragColor = vec4(irradiance, 1.0);
}

