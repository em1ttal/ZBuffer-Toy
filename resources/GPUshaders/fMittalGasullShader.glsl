#version 330

in vec4 normal;
in vec4 V;
in vec4 L_vectors[5];
in vec4 H_vectors[5];

uniform vec3 globalAmbientLight;
uniform int numLights;

struct Light{
    vec3 Ia; // Component ambiental
    vec3 Id; // Component difusa
    vec3 Is; // Component especular
    vec3 coefficients;
    vec4 position;
    
};
uniform Light light[5];

struct Material{
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float shininess;
    float opacity;
};
uniform Material material;

out vec4 colorOut;

void main()
{
    vec4 L, H, N = normal;
    vec3 diffuse, specular, ambient, shading = vec3(0.0, 0.0, 0.0);
    Light currLight;
    float d, rimDot, spotDot, intensity;

    for(int i = 0; i < numLights; i++) {
        L = L_vectors[i];
        H = H_vectors[i];
        currLight = light[i];

        intensity = dot(N, L);

        if(intensity > 0.95f){
            diffuse = material.Kd;
        } else if(intensity > 0.5f){
            diffuse = material.Kd * 0.6;
        } else if(intensity > 0.25f){
            diffuse = material.Kd * 0.4;
        } else {
            diffuse = material.Kd * 0.2;
        }

        if (1 - dot(V, N) > 0.75) {
            rimDot = 1.0;
        } else {
            rimDot = 0.0;
        }

        if (pow(max(0.0, dot(N, H)), material.shininess) > 0.95) {
            spotDot = 1.0;
        } else {
            spotDot = 0.0;
        }

        ambient = currLight.Ia * material.Ka;
        specular = currLight.Is * material.Ks * spotDot;
        specular += currLight.Is * material.Kd * rimDot * intensity;

        shading += ambient + specular + diffuse;
    }
    shading += globalAmbientLight * material.Ka;
    colorOut = vec4(shading, 1.0);
}