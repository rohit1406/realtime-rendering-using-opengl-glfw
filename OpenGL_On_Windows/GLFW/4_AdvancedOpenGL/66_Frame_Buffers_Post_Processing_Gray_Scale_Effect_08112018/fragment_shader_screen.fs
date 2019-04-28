#version 330 core

//in parameters
in vec2 out_texcoords;

//out parameters
out vec4 FragColor;

//uniforms
uniform sampler2D u_screen_texture;

//entry-point function
void main()
{
//inverset the texture color
vec3 col = vec3(texture(u_screen_texture, out_texcoords));
//float average = (col.r + col.g + col.b) / 3.0;
//since human eye tends to be more sensitive to green color and least to blue
float average = 0.2126 * col.r + 0.7152* col.g + 0.0722 * col.b;
FragColor = vec4(average,average,average, 1.0);
}
