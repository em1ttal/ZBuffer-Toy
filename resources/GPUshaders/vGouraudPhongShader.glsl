#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec4 vNormal;

uniform mat4 model_view;
uniform mat4 projection;
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

out vec4 color;

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;

    vec3 shading = vec3(0.0, 0.0, 0.0);
    vec3 ambient_part = vec3(0.0, 0.0, 0.0);
    vec3 diffuse_part = vec3(0.0, 0.0, 0.0);
    vec3 specular_part = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < numLights; i++) {
        vec4 L = normalize(light[i].position - vPosition);
        vec4 N = normalize(vNormal);
        vec4 V = normalize(obs - vPosition);
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
    }
    shading = ambient_part + diffuse_part + specular_part;

    color = vec4(shading, 1.0);
}