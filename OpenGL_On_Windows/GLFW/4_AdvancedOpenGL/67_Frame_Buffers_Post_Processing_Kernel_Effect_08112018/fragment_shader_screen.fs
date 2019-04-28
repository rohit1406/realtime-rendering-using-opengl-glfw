#version 330 core

//in parameters
in vec2 out_texcoords;

//out parameters
out vec4 FragColor;

//uniforms
uniform sampler2D u_screen_texture;

//global variables
const float offset = 1.0 / 300.0;

//entry-point function
void main()
{
//offset matrix
vec2 offsets[9] = vec2[](
					vec2(-offset, offset), //top-left
					vec2(0.0, offset), //top-center
					vec2(offset, offset), //top-right
					vec2(-offset, 0.0), //left-center
					vec2(0.0, 0.0), //center
					vec2(offset, 0.0), //right-center
					vec2(-offset, -offset), //left-bottom
					vec2(0.0, -offset), //bottom-center
					vec2(offset, -offset) //right-bottom
					);
	
//kernel matrix	
float kernel[9] = float[](
				-1, -1, -1,
				-1, 9, -1,
				-1, -1, -1
				);
	
//store sample texels from the surrounding area	
vec3 sampleTex[9];
for(int i=0; i<9; i++)
{
	sampleTex[i] = vec3(texture(u_screen_texture, out_texcoords.st + offsets[i]));
	
}

vec3 col = vec3(0.0);

//multiply sample texels with the kernel matrix
for(int i=0; i<9; i++)
{
	col += sampleTex[i] * kernel[i];
	
}
FragColor = vec4(col, 1.0);
}
