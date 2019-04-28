#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess; //scattering or radius of the specular lighting
};

//for light properties
struct Light
{
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;

//required for attenuation
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
uniform Light light;

void main()
{
//multiply light color with this ambient material color
//0.1 is ambient intensity
vec3 ambient_lighting = light.ambient * vec3(texture(material.diffuse, out_texcoords));

//diffuse lighting calculations
//normalizing because we don't care about the light magnitude or position of the vector, we only care about the direction
//so all calculations are done using unit vectors
vec3 norm = normalize(out_normals);
vec3 light_direction = normalize(u_light_position - out_frag_pos); //light's direction vector

//calculate diffuse impact
//if angle is greater than 90 then dot product will become negative and we end up with negative diffuse component
//so we are using max, because lighting for -ve values in not defined
float diff = max(dot(norm, light_direction),0.0);

//diffuse lighting
vec3 diffuse_lighting = light.diffuse * (diff * vec3(texture(material.diffuse, out_texcoords)));

//calculate specular intensity
float specularStrength = 0.5;
vec3 viewer_direction = normalize(u_viewer_position - out_frag_pos); //viewer direction vector
//reflect vector along the normal axis
//reverse the direction of the light_direction vector so that it comes from light source to the fragment position, currently it is from frag pos to light source
vec3 reflect_direction = reflect(-light_direction, norm);
float spec = pow(max(dot(viewer_direction, reflect_direction), 0.0), material.shininess);
//0.5 is a specular strength
vec3 specular_lighting = light.specular * (spec * vec3(texture(material.specular, out_texcoords)));

//calculate attenuation
float distance = length(light.position - out_frag_pos);
float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

ambient_lighting = ambient_lighting * attenuation;
diffuse_lighting = diffuse_lighting * attenuation;
specular_lighting = specular_lighting * attenuation;

//final color
vec3 final_color = ambient_lighting + diffuse_lighting + specular_lighting;

FragColor = vec4(final_color,1.0);
}