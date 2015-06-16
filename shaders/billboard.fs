#version 330 core

in vec2 UVcoords;

uniform sampler2D textureSampler;

out vec4 FragColor;

void main()
{
    FragColor = texture2D(textureSampler, UVcoords);
}