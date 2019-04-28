#version 330 core

in vec2 out_texcoords;

out vec4 FragColor;

uniform sampler2D u_depthMap;
uniform float near_plane;
uniform float far_plane;

float linearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; //back to NDC
	return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}
void main()
{
	float depthValue = texture(u_depthMap, out_texcoords).r;
	//FragColor = vec4(vec3(linearizeDepth(depthValue) / far_plane), 1.0); //perspective
	FragColor = vec4(vec3(depthValue), 1.0); //orthographic
}