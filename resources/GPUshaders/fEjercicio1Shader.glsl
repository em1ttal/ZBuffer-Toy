#version 330

in vec4 pos;
in vec4 normals;
in vec2 texture_mesh;

uniform sampler2D texMap;
uniform vec4 globalAmbientLight;
uniform vec4 obs;
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

out vec4 colorOut;

void main() {
    vec3 goraud_color = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < numLights; i++) {
        vec3 N = normalize(normals.xyz);
        vec3 L = normalize(light[i].position.xyz - pos.xyz);
        vec3 V = normalize(obs.xyz - pos.xyz);
        vec3 H = normalize(L + V);
        float d = length(L);

        float att = 1.0 / ((light[i].coefficients.x * d * d) + (light[i].coefficients.y * d) + light[i].coefficients.z);
        vec3 diffuse = light[i].Id * material.Kd * max(dot(N, L), 0.0);
        vec3 specular = light[i].Is * material.Ks * pow(max(dot(N, H), 0.0), material.shininess);
        vec3 ambient = light[i].Ia * material.Ka;

        goraud_color += (att * (diffuse + specular)) + ambient;
    }
    goraud_color += globalAmbientLight.rgb * material.Ka;

    vec4 texColor = texture(texMap, texture_mesh);
    vec3 sepia = vec3(
        texColor.r * 0.393 + texColor.b * 0.769 + texColor.g * 0.189,
        texColor.r * 0.349 + texColor.b * 0.686 + texColor.g * 0.168,
        texColor.r * 0.272 + texColor.b * 0.534 + texColor.g * 0.131
    );

    vec3 finalColor = sepia * goraud_color;

    if (sqrt(pow(pos.x, 2.0) + pow(pos.y, 2.0)) <= 0.86) {
        if (sqrt(pow(pos.x, 2.0) + pow(pos.y, 2.0)) <= 0.84) {
            colorOut = vec4(finalColor, 1.0);
        } else {
            vec3 circleColor = vec3(1.0, 0.65, 0.0);
            colorOut = vec4(circleColor, 1.0);
        }
    } else {
        colorOut = vec4(goraud_color * material.Kd, 1.0);
    }
}
