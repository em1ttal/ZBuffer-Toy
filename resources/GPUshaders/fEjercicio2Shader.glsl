#version 330

in vec4 pos;
in vec4 normal;
in float timer;

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

vec3 calcGouraudColor() {
    vec3 goraoud_color = (globalAmbientLight.xyz * material.Ka);

    for(int i = 0; i < numLights; i++){
        vec3 N = normalize(normal.xyz);
        vec3 L = normalize(light[i].position.xyz - pos.xyz);
        vec3 V = normalize(obs.xyz - pos.xyz);
        vec3 H = normalize(L + V);
        float d = length(L);
        float att = 1.0f/((light[i].coefficients.x * d*d) + (light[i].coefficients.y * d) + light[i].coefficients.z);
        vec3 difusa = light[i].Id * material.Kd* max(dot(N, L), 0.0);
        vec3 especular = light[i].Is * material.Ks * pow(max(dot(N, H), 0.0), material.shininess);
        vec3 ambiente = light[i].Ia * material.Ka;
        goraoud_color += att * (difusa + especular) + ambiente;
    }
    return goraoud_color;
}

vec3 calcToonColor() {
    vec3 shading = vec3(0.0, 0.0, 0.0);
    vec3 N = normalize(normal.xyz);
    vec3 V = normalize(obs.xyz - pos.xyz);
    float dotVN = dot(V, N);
    float rimDot = 0.0;
    float spotDot = 0.0;

    if ((1.0 - dotVN) > 0.75) {
        rimDot = 1.0;
    }

    for (int i = 0; i < numLights; i++) {
        vec3 L = normalize(light[i].position.xyz - pos.xyz);
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

    return shading;
}

void main()
{
    vec3 color = calcGouraudColor();
    float factor = sqrt(color.r*color.r + color.b*color.b + color.g*color.g)/3.0;
    vec3 gris = vec3(factor, factor, factor);

    float ratio = 1.0f - timer;
    if (sqrt(pow(pos.x,2.0) + pow(pos.y, 2.0)) + pow(pos.z, 2.0) < ratio){
        colorOut = vec4(gris, 1.0f);
    }
    else{
        colorOut = vec4(calcToonColor(), 1.0);   
    }
}