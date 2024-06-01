#version 330 core

in vec4 position;
in vec4 color;
in vec4 normal;

out vec4 colorOut;

uniform vec4 obs;
uniform vec3 globalAmbientLight;
uniform int numLights;

struct Light {
    vec3 Ia; // Ambient component
    vec3 Id; // Diffuse component
    vec3 Is; // Specular component
    vec3 coefficients;
    vec4 position;   
};
uniform Light light[5];

struct Material {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float shininess;
    float opacity;
};
uniform Material material;

void main() {
    vec3 shading = vec3(0.0, 0.0, 0.0);
    vec3 N = normalize(normal.xyz);
    vec3 V = normalize(obs.xyz - position.xyz);
    float dotVN = dot(V, N);
    float rimDot = 0.0;
    float spotDot = 0.0;

    if ((1.0 - dotVN) > 0.75) {
        rimDot = 1.0;
    }

    for (int i = 0; i < numLights; i++) {
        vec3 L = normalize(light[i].position.xyz - position.xyz);
        float intensity = dot(N, L);
        vec3 H = normalize(L + V);
        float dotNH = dot(N, H);

        if (pow(max(0.0, dotNH), material.shininess) > 0.95) {
            spotDot = 1.0;
        } else {
            spotDot = 0.0;
        }

        if (intensity > 0.95) {
            shading += material.Kd;
        } else if (intensity > 0.5) {
            shading += material.Kd * 0.6;
        } else if (intensity > 0.25) {
            shading += material.Kd * 0.4;
        } else {
            shading += material.Kd * 0.2;
        }
        
        // Specular component
        shading += light[i].Is * material.Ks * spotDot;

        // Rim lighting
        shading += material.Kd * material.Ks * rimDot * intensity;

        // Ambient component
        shading += light[i].Ia * material.Ka;
    }

    shading += globalAmbientLight * material.Ka;
    shading = clamp(shading, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));

    colorOut = vec4(shading, 1.0);
}
