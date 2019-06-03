#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

out vec3 out_Color;
out vec2 out_texcoord;

uniform float u_position_offset;
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;
void main()
{//u_projection_matrix * u_view_matrix *
    gl_Position =   u_projection_matrix * u_view_matrix * u_model_matrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
    out_Color = aColor;
	out_texcoord = aTexCoords;
}