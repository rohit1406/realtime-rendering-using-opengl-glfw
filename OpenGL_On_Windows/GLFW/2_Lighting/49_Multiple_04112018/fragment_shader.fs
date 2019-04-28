#version 330 core

//macros
#define NR_POINT_LIGHTS 4

//global variables
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess; //scattering or radius of the specular lighting
};

//for directional light
struct DirLight
{
vec3 direction; //light direction
vec3 ambient;
vec3 diffuse;
vec3 specular;
};

//for point light
struct PointLight
{
vec3 position; //spot light position vector to calculate light's direction vector

vec3 ambient;
vec3 diffuse;
vec3 specular;

//constants
float constant;
float linear;
float quadratic;
};

//in parameters
in vec3 out_normals;
in vec3 out_frag_pos;
in vec2 out_texcoords;

//out parameters
out vec4 FragColor;

//uniforms
uniform vec3 u_object_color;
uniform vec3 u_light_color;
uniform vec3 u_light_position;
uniform vec3 u_viewer_position;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight[NR_POINT_LIGHTS];

//function prototype declaration
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
//normalizing because we don't care about the light magnitude or position of the vector, we only care about the direction
//so all calculations are done using unit vectors
vec3 norm = normalize(out_normals);

//calculate viewer direction
vec3 viewer_direction = normalize(u_viewer_position - out_frag_pos); //viewer direction vector
//final color
vec3 final_color;

//phase 1 - directional lighting
final_color = CalcDirLight(dirLight, norm, viewer_direction);

//phase 2 - point lights
for(int i=0; i<NR_POINT_LIGHTS; i++)
{
	final_color += CalcPointLight(pointLight[i], norm, out_frag_pos, viewer_direction);
}

//phase 3 - spot lights
//final_color = CalcSpotLight(spotLight, norm, out_frag_pos, viewer_direction);
FragColor = vec4(final_color,1.0);
}


//CalcDirLight calculates the color impact of the directional light
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	//negating to make it a direction vector pointing towards the light source from the fragment
	vec3 light_direction = normalize(normalize(-light.direction)); //light's direction vector
	
	//calculate diffuse shading
	//if angle is greater than 90 then dot product will become negative and we end up with negative diffuse component
	//so we are using max, because lighting for -ve values in not defined
	float diff = max(dot(normal, light_direction),0.0);
	
	//calculate specular shading
	//reflect vector along the normal axis
	//reverse the direction of the light_direction vector so that it comes from light source to the fragment position, currently it is from frag pos to light source
	vec3 reflect_direction = reflect(-light_direction, normal);
	float spec = pow(max(dot(viewDir, reflect_direction), 0.0), material.shininess);
	
	//combine result
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, out_texcoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, out_texcoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, out_texcoords));
	
	//directional light impact - color
	vec3 result = ambient + diffuse + specular;
	return result;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 light_direction = normalize(light.position - fragPos); //light's direction vector
	
	//calculate diffuse shading
	//if angle is greater than 90 then dot product will become negative and we end up with negative diffuse component
	//so we are using max, because lighting for -ve values in not defined
	float diff = max(dot(normal, light_direction),0.0);
	
	//calculate specular shading
	//reflect vector along the normal axis
	//reverse the direction of the light_direction vector so that it comes from light source to the fragment position, currently it is from frag pos to light source
	vec3 reflect_direction = reflect(-light_direction, normal);
	float spec = pow(max(dot(viewDir, reflect_direction), 0.0), material.shininess);
	
	//attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	//combine result
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, out_texcoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, out_texcoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, out_texcoords));
	ambient = ambient * attenuation;
	diffuse = diffuse * attenuation;
	specular = specular * attenuation;
	
	//directional light impact - color
	vec3 result = ambient + diffuse + specular;
	return result;
}