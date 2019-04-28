#version 330 core

//in parameters
in vec2 out_texcoords;
in vec3 out_position;
in vec3 out_normal;

//out parameters
out vec4 FragColor;

//uniforms
uniform samplerCube u_skybox_sampler;
uniform vec3 u_camera_position;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;
uniform sampler2D texture_ambient;

//entry-point function
void main()
{
//viewer direction vector
vec3 I = normalize(out_position - u_camera_position);
//reflection vector
vec3 R = reflect(I, normalize(out_normal));

vec3 reflectColor = texture(u_skybox_sampler, R).rgb;

vec3 reflectOnModel =  texture(texture_ambient, out_texcoords).rgb;

vec3 sumReflect = mix(reflectColor, reflectOnModel, 0.5);

vec3 modelTextureColor = 0.34 * texture(texture_diffuse, out_texcoords).rgb
						+ 0.75 * sumReflect * texture(texture_specular, out_texcoords).rgb;
						
FragColor = vec4(modelTextureColor, 1.0);
}
