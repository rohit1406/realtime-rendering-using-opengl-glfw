#version 450 core

//in parameters
in VS_OUT
{
 vec3 out_frag_pos;
 vec3 out_normals;
 vec2 out_texcoords;
 vec4 out_fragpos_light_space;
} fs_in;

//out parameters
out vec4 FragColor;

//uniforms
uniform vec3 u_light_position;
uniform vec3 u_viewer_position;
uniform sampler2D u_sampler_diffuse_texture;
uniform sampler2D u_sampler_shadow_map;
uniform bool blinn;

//global function declaration
float shadowCalculation(vec4 fragPosLightSpace)
{
    //perform perspective division - convert to NDC
	vec3 projCoords = fs_in.out_fragpos_light_space.xyz / fs_in.out_fragpos_light_space.w;
	
	//transform to range 0,1
	projCoords = projCoords * 0.5 + 0.5;
	
	//closest depth from the lights point of viewer
	float closestDepth = texture(u_sampler_shadow_map, projCoords.xy).r;
	
	//depth of the fragment from lights perspective
	float currentDepth = projCoords.z;
	
	//check if currentDepth is higher that closestDepth
	vec3 norm = normalize(fs_in.out_normals);
	vec3 light_direction = normalize(u_light_position - fs_in.out_frag_pos); //light's direction vector
	float bias = max(0.05 * (1.0 - dot(norm, light_direction)),0.005);
	//float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	/*PCF - Percentage-Closer filtering*/
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(u_sampler_shadow_map, 0);
	for(int x=-1; x <= 1; ++x)
	{
		for(int y=-1; y<=1; ++y)
		{
			float pcfDepth = texture(u_sampler_shadow_map, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;
	
	if(projCoords.z > 1.0)
        shadow = 0.0;
	
	return shadow;
}

void main()
{
	vec3 color = texture(u_sampler_diffuse_texture, fs_in.out_texcoords).rgb;

//ambient	
float ambient_factor = 0.15; //a constant ambient factor
vec3 lightColor = vec3(1.0);
//multiply object color by this ambient color
vec3 ambient_lighting = ambient_factor * color;

//diffuse lighting calculations
//normalizing because we don't care about the light magnitude or position of the vector, we only care about the direction
//so all calculations are done using unit vectors
vec3 norm = normalize(fs_in.out_normals);
vec3 light_direction = normalize(u_light_position - fs_in.out_frag_pos); //light's direction vector

//calculate diffuse impact
//if angle is greater than 90 then dot product will become negative and we end up with negative diffuse component
//so we are using max, because lighting for -ve values in not defined
float diff = max(dot(light_direction, norm),0.0);

//diffuse lighting
vec3 diffuse_lighting = diff * lightColor;

//calculate specular intensity
vec3 viewer_direction = normalize(u_viewer_position - fs_in.out_frag_pos); //viewer direction vector
//reflect vector along the normal axis
//reverse the direction of the light_direction vector so that it comes from light source to the fragment position, currently it is from frag pos to light source

float spec = 0.0;

if(blinn)
{
	vec3 halfway_direction = normalize(light_direction + viewer_direction);
	spec = pow(max(dot(norm, halfway_direction), 0.0), 32.0);

}else
{
	vec3 reflect_direction = reflect(-light_direction, norm);
	spec = pow(max(dot(viewer_direction, reflect_direction), 0.0), 8.0);
}
vec3 specular_lighting = spec * lightColor;

//calculate shadow
float shadow = shadowCalculation(fs_in.out_fragpos_light_space);

//final color
vec3 final_color = (ambient_lighting + (1.0 - shadow) * (diffuse_lighting + specular_lighting)) * color;

FragColor = vec4(final_color,1.0);

//gamma correction
//float gamma = 2.2;
//FragColor.rgb = pow(final_color.rgb, vec3(1.0/gamma));
}