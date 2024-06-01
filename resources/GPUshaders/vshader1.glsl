#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

uniform mat4 model_view;
uniform mat4 projection;

uniform vec3 globalAmbientLight;

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

    //color = vColor;

    //1.1
    //color = vec4(globalAmbientLight, 1);

    //1.2
    color = vec4(light[0].Ia + globalAmbientLight, 1);
}
