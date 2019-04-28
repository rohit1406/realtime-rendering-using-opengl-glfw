#version 330 core

//in parameters
in vec2 out_texcoords;

//out parameters
out vec4 FragColor;

//uniforms
uniform sampler2D u_texture0;

//global variables
float near = 0.1;
float far = 100.0;

//global function prototype declaration
float linearizeDepth(float depth);

//entry-point function
void main()
{
float depth = linearizeDepth(gl_FragCoord.z) / far;
FragColor = vec4(vec3(depth), 1.0);;
}

float linearizeDepth(float depth)
{
float z = depth * 2.0 - 1.0; //back to NDC [-1,1]
return (2.0 * near * far) / (far + near - z * (far - near));
}