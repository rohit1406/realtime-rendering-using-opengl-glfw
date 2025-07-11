#version 330 core

//global variables
const float PI = 3.14159265359;

//in params
in vec2 out_texcoords;
in vec3 out_world_pos;
in vec3 out_normals;

//uniforms
//materials
uniform vec3 u_albedo;
uniform float u_mettalic;
uniform float u_roughness;
uniform float u_ao;

//lights
uniform vec3 u_light_positions[4];
uniform vec3 u_light_colors[4];
uniform vec3 u_viewer_position;

//out params
out vec4 FragColor;

//method signatures
float distributionGGX(vec3 N, vec3 H, float roughness);
float geometrySchlickGGX(float nDotV, float roughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

//entry-point function
void main()
{
	vec3 N = out_normals;
	vec3 V = normalize(u_viewer_position - out_world_pos);
	//vec3 R = reflect(-V, N);
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, u_albedo, u_mettalic);
	
	//reflectance equation
	vec3 L0 = vec3(0.0);
	for(int i=0; i<4; i++)
	{
		//calculate per-light radiance
		vec3 L = normalize(u_light_positions[i] - out_world_pos);
		vec3 H = normalize(V + L);
		float distance = length(u_light_positions[i] - out_world_pos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = u_light_colors[i] * attenuation;
		
		//Cook-Torrance BRDF
		float NDF = distributionGGX(N, H, u_roughness);
		float G = geometrySmith(N, V, L, u_roughness);
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
		
		vec3 nominator = NDF * G * F;
		float denominator = 4 * max(dot(N,V), 0.0) * max(dot(N,L), 0.0) + 0.001;
		vec3 specular = nominator / denominator; //prevent divide by 0
		
		//kS = Fresnel
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - u_mettalic;
		
		//scale light
		float NdotL = max(dot(N, L), 0.0);
		
		L0 += (kD * u_albedo / PI + specular) * radiance * NdotL;
	}
	
	vec3 ambient = vec3(0.03) * u_albedo * u_ao;
	vec3 color = ambient + L0;
	
	//HDR tonemapping
	color = color / (color + vec3(1.0));
	
	//gamma correctg
	color = pow(color, vec3(1.0/2.2));
	
	FragColor = vec4(color, 1.0);
}


float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}
// ----------------------------------------------------------------------------
float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}