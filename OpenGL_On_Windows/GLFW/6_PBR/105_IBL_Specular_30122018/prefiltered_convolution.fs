#version 330 core

//global variables
const float PI = 3.14159265359;

//in params
in  vec3 out_local_pos;

//uniforms
//materials
uniform samplerCube u_sampler_environment_map;
uniform float u_roughness;

//out params
out vec4 FragColor;

//method signatures
float radicalInverse_vdc(uint bits);
vec2 hammersley(uint i, uint N);
vec3 importanceSampleGGX(vec2 xi, vec3 N, float roughness);

//entry-point function
void main()
{
	vec3 N = normalize(out_local_pos);
	vec3 R = N;
	vec3 V = R;
	
	const uint SAMPLE_COUNT = 1024u;
	float totalWeight = 0.0;
	vec3 prefilteredColor = vec3(0.0);
	
	for(uint i=0u; i< SAMPLE_COUNT; i++)
	{
		vec2 xi = hammersley(i, SAMPLE_COUNT);
		vec3 H = importanceSampleGGX(xi, N, u_roughness);
		vec3 L = normalize(2.0 * dot(V, H) * H - V);
		
		float NdotL = max(dot(N, L), 0.0);
		if(NdotL > 0.0)
		{
			prefilteredColor += texture(u_sampler_environment_map, L).rgb * NdotL;
			totalWeight += NdotL;
		}
	}
	prefilteredColor = prefilteredColor / totalWeight;
	
	FragColor = vec4(prefilteredColor, 1.0);
}

float radicalInverse_vdc(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// ----------------------------------------------------------------------------
vec2 hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), radicalInverse_vdc(i));
}  

vec3 importanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness*roughness;
	
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
    // from spherical coordinates to cartesian coordinates
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
	
    // from tangent-space vector to world-space sample vector
    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
	
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}  