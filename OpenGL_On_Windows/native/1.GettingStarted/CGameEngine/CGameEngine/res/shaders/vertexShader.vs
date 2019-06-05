#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 out_normal;
out vec2 out_texcoord;
out vec3 out_frag_pos;

uniform float u_position_offset;
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

void main()
{
	vec4 worldPos = u_model_matrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
    gl_Position =   u_projection_matrix * u_view_matrix * worldPos;
    out_normal = mat3(transpose(inverse(u_model_matrix))) * aNormal; // will work for non-uniform scaling also
	out_frag_pos = worldPos.xyz;
	out_texcoord = aTexCoords;
}