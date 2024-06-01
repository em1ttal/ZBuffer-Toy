#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vNormal;

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

out vec4 normal;
out vec4 V;
out vec4 L_vectors[5];
out vec4 H_vectors[5];

void main()
{
    vec4 light_position, vertex_position, camera_position;
    vertex_position = model_view*vPosition;
    gl_Position = projection*vertex_position;
    gl_Position = gl_Position/gl_Position.w;

    normal = model_view*vNormal;

    camera_position = model_view*obs;
    camera_position = camera_position/camera_position.w;
    V = normalize(camera_position - vertex_position);

    for(int i = 0; i < numLights; i++) {
        light_position = model_view*light[i].position;
        light_position = light_position/light_position.w;

        L_vectors[i] = normalize(light_position - vertex_position);
        H_vectors[i] = normalize(L_vectors[i] + V);
    }
}