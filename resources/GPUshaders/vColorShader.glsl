#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec4 vNormal;

uniform mat4 model_view;
uniform mat4 projection;

struct Material{
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float shininess;
    float opacity;
};
uniform Material material;

struct Light{
    vec3 Ia; // Component ambiental
    vec3 Id; // Component difusa
    vec3 Is; // Component especular
    vec3 coefficients;
    vec4 position;
};
uniform Light light[5];

out vec4 color;

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;

    color = vec4(material.Kd, 1);
}