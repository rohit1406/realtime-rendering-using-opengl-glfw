#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices=6) out;

//in params
in VS_OUT
{
	vec3 normal;
}gs_in[];

//global function declarations
void GenerateLine(int index);

//global variables
const float MAGNITUDE = 0.05;

//entry-point function
void main()
{
	GenerateLine(0); //first vertex normal
	GenerateLine(1); //second vertex normal
	GenerateLine(2); //third vertex normal
	
}

void GenerateLine(int index)
{
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
	EmitVertex();
	EndPrimitive();
}