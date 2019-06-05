#version 330 core

in vec3 out_normal;
in vec2 out_texcoord;
in vec3 out_frag_pos;

out vec4 FragColor;

uniform sampler2D u_bricks_sampler;
uniform sampler2D u_face_sampler;
uniform float u_mix_texture_param;

uniform vec3 u_viewer_position;


struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light; 

uniform Material material;

void main()
{

	vec4 wood =  texture(u_bricks_sampler, out_texcoord);
	vec4 face =  texture(u_face_sampler, out_texcoord);
    //FragColor = wood * face * vec4(out_Color, 1.0f);
	vec4 objectColor = mix(wood, face, u_mix_texture_param);

	// ambient lighting
	vec3 ambient = light.ambient * material.ambient;

	// diffuse lighting
	vec3 norm = normalize(out_normal);
	vec3 lightDir = normalize(light.position - out_frag_pos); // get light direction vector
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * material.diffuse) * light.diffuse;
	
	// specular lighting
	vec3 viewDir = normalize(u_viewer_position - out_frag_pos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (spec * material.specular) * light.specular;

	//vec4 result = vec4(ambient + diffuse, 1.0) * objectColor;
	vec3 result = ambient + diffuse + specular;
	 FragColor = vec4(result, 1.0);
}