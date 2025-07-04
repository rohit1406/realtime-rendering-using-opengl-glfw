#version 330 core

out vec4 FragColor;

in vec2 out_texcoords;

uniform sampler2D u_sampler_gPositionCBO;
uniform sampler2D u_sampler_gNormalCBO;
uniform sampler2D u_sampler_gAlbedoSpecCBO;
uniform sampler2D u_sampler_ssao;

struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
};
uniform Light u_lights;

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = texture(u_sampler_gPositionCBO, out_texcoords).rgb;
    vec3 Normal = texture(u_sampler_gNormalCBO, out_texcoords).rgb;
    vec3 Diffuse = texture(u_sampler_gAlbedoSpecCBO, out_texcoords).rgb;
    float AmbientOcclusion = texture(u_sampler_ssao, out_texcoords).r;
    
    // then calculate lighting as usual
    vec3 ambient = vec3(0.3 * Diffuse * AmbientOcclusion);
    vec3 lighting  = ambient; 
    vec3 viewDir  = normalize(-FragPos); // viewpos is (0.0.0)
    // diffuse
    vec3 lightDir = normalize(u_lights.Position - FragPos);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * u_lights.Color;
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 8.0);
    vec3 specular = u_lights.Color * spec;
    // attenuation
    float distance = length(u_lights.Position - FragPos);
    float attenuation = 1.0 / (1.0 + u_lights.Linear * distance + u_lights.Quadratic * distance * distance);
    diffuse *= attenuation;
    specular *= attenuation;
    lighting += diffuse + specular;

    FragColor = vec4(lighting, 1.0);
}