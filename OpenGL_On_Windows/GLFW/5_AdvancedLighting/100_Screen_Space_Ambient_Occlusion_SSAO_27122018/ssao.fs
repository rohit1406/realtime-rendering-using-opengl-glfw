#version 330 core

//global variables
int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;
const vec2 noiseScale = vec2(800.0/4, 600.0/4);

//in attributes
in vec2 out_texcoords;

//uniforms
uniform sampler2D u_sampler_gPositionCBO;
uniform sampler2D u_sampler_gNormalCBO;
uniform sampler2D u_sampler_texNoise;
uniform vec3 u_samples[64];
uniform mat4 u_perspective_matrix;

//out params
out float FragColor;

void main()
{
	//retrieve data from gbuffer
	vec3 FragPos = texture(u_sampler_gPositionCBO, out_texcoords).rgb;
	vec3 Normal = normalize(texture(u_sampler_gNormalCBO, out_texcoords).rgb);
	vec3 RandomVec = normalize(texture(u_sampler_texNoise, out_texcoords * noiseScale).rgb);
	
	//create TBN change-of-basis matrix; from tangent space to view space
	vec3 Tangent = normalize(RandomVec - Normal * dot(RandomVec, Normal));
	vec3 Bitangent = cross(Normal, Tangent);
	mat3 TBN = mat3(Tangent, Bitangent, Normal);
	
	//Iterate Over the sample kernel and calculate occlusion factor
	float occlusion = 0.0;
	for(int i=0; i<kernelSize; i++)
	{
		//get sample position
		vec3 sample = TBN * u_samples[i]; //from tangent to view space
		sample = FragPos + sample * radius;
		
		//project sample position to sample texture to get position on screen/texture
		vec4 offset = vec4(sample, 1.0);
		offset = u_perspective_matrix * offset; //from view to clip
		offset.xyz /= offset.w; //perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5; //transform to 0,1
		
		//get sample depth
		float sampleDepth = texture(u_sampler_gPositionCBO, offset.xy).z; //get depth value of kernel sample
		
		//range check and accumulate
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(FragPos.z - sampleDepth));
		occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;
	}
	occlusion = 1.0 - (occlusion / kernelSize);
	FragColor = occlusion;
}