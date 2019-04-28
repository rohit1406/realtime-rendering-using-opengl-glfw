#version 330 core

//out parameters
out vec4 FragColor;

//entry-point function
void main()
{
if(gl_FragCoord.x < 400)
	FragColor = vec4(0.0, 1.0, 0.0, 1.0);
else
	FragColor = vec4(0.0, 0.0, 1.0, 1.0);
	

}
