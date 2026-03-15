#version 330 core

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

// A structure to hold the directional light properties in the Shader file
struct DirectionalLight{
	vec3 direction;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Get some input from the main.cpp program
uniform DirectionalLight dirLight;
// Needed for specular light calcualtion
uniform vec3 viewPos;

uniform vec3 objectColor;



void main(){
	vec3 norm = normalize(Normal);

	// Note: We negate the light direction because we need a vector pointing TOWARD the light source
    vec3 lightDir = normalize(-dirLight.direction);
	
	vec3 ambient = dirLight.ambient * objectColor;

	// 3. Diffuse (The Dot Product!)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dirLight.diffuse * diff * objectColor;
    
    // 4. Specular
    vec3 viewDir = normalize(viewPos - FragPos); // Vector pointing from pixel to camera
    vec3 reflectDir = reflect(-lightDir, norm);  // How the light bounces off the surface
    
    // The '32' is the shininess value. Lower = spread out, Higher = sharp point
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); 
    vec3 specular = dirLight.specular * spec * objectColor;
    
    // 5. Combine them all
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}