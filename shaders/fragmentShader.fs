#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
out vec4 FragColor;

// A structure to hold the directional light properties in the Shader file
struct DirectionalLight{
	vec3 direction;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

// Get some input from the main.cpp program
uniform DirectionalLight dirLight;
uniform SpotLight spotLight;
// Needed for specular light calcualtion
uniform vec3 viewPos;

uniform vec3 objectColor;
uniform sampler2D diffuseTexture;
uniform bool useTexture;

vec3 getBaseColor() {
    if (useTexture) {
        return texture(diffuseTexture, TexCoord).rgb;
    }
    return objectColor;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    vec3 baseColor = getBaseColor();

    vec3 ambient = light.ambient * baseColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * baseColor;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * baseColor;

    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 baseColor = getBaseColor();
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * baseColor;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * baseColor;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * baseColor;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}


void main(){
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = calculateDirectionalLight(dirLight, norm, viewDir);
    result += calculateSpotLight(spotLight, norm, FragPos, viewDir);
    FragColor = vec4(result, 1.0);
}
