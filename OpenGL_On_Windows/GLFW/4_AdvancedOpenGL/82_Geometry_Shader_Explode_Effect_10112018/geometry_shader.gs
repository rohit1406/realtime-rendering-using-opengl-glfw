#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

//in params
in VS_OUT
{
	vec2 texCoords;
}gs_in[];

//out params
out vec2 out_texCoords;

//uniforms
uniform float time;

//global function declarations
vec3 GetNormal();
vec4 explode(vec4 position, vec3 normal);

//entry-point function
void main()
{
	vec3 normal = GetNormal();
	
	gl_Position = explode(gl_in[0].gl_Position, normal);
	out_texCoords = gs_in[0].texCoords;
	EmitVertex();
	gl_Position = explode(gl_in[1].gl_Position, normal);
	out_texCoords = gs_in[1].texCoords;
	EmitVertex();
	gl_Position = explode(gl_in[2].gl_Position, normal);
	out_texCoords = gs_in[2].texCoords;
	EmitVertex();
	EndPrimitive();
	
}

vec3 GetNormal()
{
	//vectors parallel to the surface of the triangle
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	
	//derive the vector parallel to it - normal vector
	return normalize(cross(a,b));
}

vec4 explode(vec4 position, vec3 normal)
{
	float magnitude = 2.0;
	vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
	return position + vec4(direction, 0.0);
	
}