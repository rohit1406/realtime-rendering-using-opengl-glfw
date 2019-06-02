#version 330 core

in vec3 out_Color;
in vec2 out_texcoord;

out vec4 FragColor;

uniform sampler2D u_bricks_sampler;
uniform sampler2D u_face_sampler;
uniform float u_mix_texture_param;
void main()
{
	vec4 wood =  texture(u_bricks_sampler, out_texcoord);
	vec4 face =  texture(u_face_sampler, out_texcoord);
    //FragColor = wood * face * vec4(out_Color, 1.0f);
	FragColor = mix(wood, face, u_mix_texture_param);
}