#version 330 core

in vec3 color;
out vec4 FragColor;

uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor * color, 1.0);
}


