#version 450 core

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoords;

layout(location = 0) out vec4 color;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform vec3 baseColor;
uniform float ambient;
uniform float diffuse;
uniform float specular;
uniform float specStrength;
uniform float specPower;

void main() {
    // Ambient
    vec3 ambientColor = ambient * lightColor;

    // Diffuse
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = diffuse * diff * lightColor;

    // Specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specPower);
    vec3 specularColor = specStrength * spec * lightColor;

    // Final color
    color = vec4(baseColor * (ambientColor + diffuseColor + specularColor), 1.0);
}
