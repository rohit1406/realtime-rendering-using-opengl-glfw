#version 330 core

//in parameters
in vec3 out_normals;
in vec3 out_frag_pos;

//out parameters
out vec4 FragColor;

//uniforms
uniform vec3 u_object_color;
uniform vec3 u_light_color;
uniform vec3 u_light_position;
uniform vec3 u_viewer_position;

void main()
{
float ambient_factor = 0.1; //a constant ambient factor

//multiply light color with this ambient strength
vec3 ambient = ambient_factor * u_light_color;

//multiply object color by this ambient color
vec3 ambient_lighting = ambient * u_object_color;

//diffuse lighting calculations
//normalizing because we don't care about the light magnitude or position of the vector, we only care about the direction
//so all calculations are done using unit vectors
vec3 norm = normalize(out_normals);
vec3 light_direction = normalize(u_light_position - out_frag_pos); //light's direction vector

//calculate diffuse impact
//if angle is greater than 90 then dot product will become negative and we end up with negative diffuse component
//so we are using max, because lighting for -ve values in not defined
float diff = max(dot(norm, light_direction),0.0);

//diffuse lighting
vec3 diffuse_lighting = diff * u_light_color;

//calculate specular intensity
float specularStrength = 0.5;
float shininess = 32;
vec3 viewer_direction = normalize(u_viewer_position - out_frag_pos); //viewer direction vector
//reflect vector along the normal axis
//reverse the direction of the light_direction vector so that it comes from light source to the fragment position, currently it is from frag pos to light source
vec3 reflect_direction = reflect(-light_direction, norm);
float spec = pow(max(dot(viewer_direction, reflect_direction), 0.0), shininess);
vec3 specular_lighting = specularStrength * spec * u_light_color;

//final color
vec3 final_color = (ambient_lighting + diffuse_lighting + specular_lighting) * u_object_color;

FragColor = vec4(final_color,1.0);
}