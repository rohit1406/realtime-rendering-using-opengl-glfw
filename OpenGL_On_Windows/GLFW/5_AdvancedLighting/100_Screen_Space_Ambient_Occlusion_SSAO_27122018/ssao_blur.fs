#version 330 core

//in attributes
in vec2 out_texcoords;

//uniforms
uniform sampler2D u_sampler_ssao_input;

//out params
out float FragColor;

void main()
{
	vec2 texelSize = 1.0 / vec2(textureSize(u_sampler_ssao_input, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(u_sampler_ssao_input, out_texcoords + offset).r;
        }
    }
    FragColor = result / (4.0 * 4.0);
}