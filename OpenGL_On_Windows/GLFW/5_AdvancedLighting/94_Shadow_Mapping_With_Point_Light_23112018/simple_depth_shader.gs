#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 u_shadow_matrices[6];

out vec4 FragPos; //output per emitVertex

void main()
{
	for(int face=0; face<6; ++face)
	{
		gl_Layer = face; //to which face we render
		for(int i=0; i<3; ++i)
		{
			FragPos = gl_in[i].gl_Position;
			gl_Position = u_shadow_matrices[face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}