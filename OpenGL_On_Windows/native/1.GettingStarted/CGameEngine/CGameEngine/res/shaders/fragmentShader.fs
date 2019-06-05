#version 330 core

in vec3 out_normal;
in vec2 out_texcoord;
in vec3 out_frag_pos;

out vec4 FragColor;

uniform sampler2D u_bricks_sampler;
uniform sampler2D u_face_sampler;
uniform float u_mix_texture_param;

uniform vec3 u_object_color;
uniform vec3 u_light_color;
uniform vec3 u_light_position;
uniform vec3 u_viewer_position;

const float AMBIENT_STRENGTH = 0.1;
const float SPECULAR_STRENGTH = 0.5;
const float SHININESS = 32;

void main()
{

	vec4 wood =  texture(u_bricks_sampler, out_texcoord);
	vec4 face =  texture(u_face_sampler, out_texcoord);
    //FragColor = wood * face * vec4(out_Color, 1.0f);
	vec4 objectColor = mix(wood, face, u_mix_texture_param);

	// ambient lighting
	vec3 ambient = AMBIENT_STRENGTH * u_light_color;

	// diffuse lighting
	vec3 norm = normalize(out_normal);
	vec3 lightDir = normalize(u_light_position - out_frag_pos); // get light direction vector
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_light_color;
	
	// specular lighting
	vec3 viewDir = normalize(u_viewer_position - out_frag_pos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), SHININESS);
	vec3 specular = SPECULAR_STRENGTH * spec * u_light_color;

	//vec4 result = vec4(ambient + diffuse, 1.0) * objectColor;
	vec4 result = vec4(ambient + diffuse + specular, 1.0) * vec4(u_object_color, 1.0);
	 FragColor = result;
}