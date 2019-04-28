#version 330 core

//in parameters
in vec3 out_position;
in vec3 out_normal;

//out parameters
out vec4 FragColor;

//uniforms
uniform samplerCube u_skybox_sampler;
uniform vec3 u_camera_position;

//entry-point function
void main()
{
//viewer direction vector
vec3 I = normalize(out_position - u_camera_position);
//reflection vector
vec3 R = reflect(I, normalize(out_normal));
FragColor = vec4(texture(u_skybox_sampler, R).rgb, 1.0);
}
