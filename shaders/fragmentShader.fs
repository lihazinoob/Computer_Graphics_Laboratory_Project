#version 330 core

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

void main(){
	vec3 norm = normalize(Normal);
	vec3 color = norm * 0.5 + 0.5;
	FragColor = vec4(color,1.0);
}