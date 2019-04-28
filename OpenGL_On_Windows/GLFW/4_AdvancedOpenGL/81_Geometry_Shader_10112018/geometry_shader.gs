#version 450 core

layout (points) in;
//layout (line_strip, max_vertices=2) out;
layout (triangle_strip, max_vertices=5) out;

in VS_OUT {
    vec3 o_color;
} gs_in[]; 

//out parameters
out vec3 out_fColor; 

//global functions
void draw_lines(vec4 position);
void build_house(vec4 position);

//entry-point function
void main()
{
	//draw_lines(gl_in[0].gl_Position);
	build_house(gl_in[0].gl_Position);
	
}

void draw_lines(vec4 position)
{
	gl_Position = position + vec4(-0.1, 0.0, 0.0, 0.0);
	EmitVertex();
	
	gl_Position = position + vec4(0.1, 0.0, 0.0, 0.0);
	EmitVertex();
	
	EndPrimitive();
}

void build_house(vec4 position)
{
	out_fColor = gs_in[0].o_color; // gs_in[0] since there's only one input vertex
	
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); //bottom-left
	EmitVertex();
	
	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0); //bottom-right
	EmitVertex();
	
	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0); //top-left
	EmitVertex();
	
	gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0); //top-right
	EmitVertex();
	
	gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0); //top-middle - roof of home
	out_fColor = vec3(1.0, 1.0, 1.0);
	EmitVertex();
	
	EndPrimitive();
}