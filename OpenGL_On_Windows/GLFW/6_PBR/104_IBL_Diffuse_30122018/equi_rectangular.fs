#version 330 core

//global params
const vec2 invAtan = vec2(0.1591, 0.3183);

//in params
in vec3 out_local_pos;

//uniforms
//materials
uniform sampler2D u_sampler_equirectangular_map;

//out params
out vec4 FragColor;

//method signatures
vec2 sampleSphericalMap(vec3 v);

//entry-point function
void main()
{
	vec2 uv = sampleSphericalMap(normalize(out_local_pos));
	vec3 color = texture(u_sampler_equirectangular_map, uv).rgb;
	FragColor = vec4(color, 1.0);
}


vec2 sampleSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}