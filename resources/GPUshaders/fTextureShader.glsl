#version 330

in vec2 textCoord;
out vec4 colorOut;

uniform sampler2D texMap; 

void main()
{
    colorOut = vec4(texture(texMap, textCoord).rgb, 1.0f);
}