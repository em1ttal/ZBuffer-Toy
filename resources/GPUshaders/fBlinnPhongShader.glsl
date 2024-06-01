#version 330

in vec4 normal;
in vec4 position;
out vec4 color;
uniform vec4 global_light;
uniform vec4 obs;

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

void main()
{
    vec3 shading = vec3(0.0, 0.0, 0.0);
    vec3 ambient_part = vec3(0.0, 0.0, 0.0);
    vec3 diffuse_part = vec3(0.0, 0.0, 0.0);
    vec3 specular_part = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < numLights; i++) {
        vec4 L = normalize(light[i].position - position);
        float d = length(L);
        vec4 N = normalize(normal);
        vec4 V = normalize(obs - position);
        vec4 H = normalize(L + V);

        vec3 light_amb = light[i].Ia;
        vec3 light_diff = light[i].Id;
        vec3 light_spec = light[i].Is;
        float light_a = light[i].coefficients.x;
        float light_b = light[i].coefficients.y;
        float light_c = light[i].coefficients.z;

        ambient_part += light_amb * material.Ka;
        diffuse_part += light_diff * material.Kd * dot(L, N);
        specular_part += light_spec * material.Ks * pow(max(dot(N, H), 0.0f), material.shininess);

        float att = 1.0f / (light_a + light_b*d + light_c*d*d);
        shading += ambient_part + (att * (diffuse_part + specular_part));
    }
    shading += material.Ka * globalAmbientLight;

    color = vec4(shading, 1.0);
}