#version 330 core

in vec3 color;
out vec4 FragColor;

uniform vec3 lightColor;
uniform float ambientStrength;

void main()
{
    
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = ambient * color;
    FragColor = vec4(result, 1.0);
}


