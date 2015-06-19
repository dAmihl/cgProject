#version 330 core

in vec2 UVcoords;

uniform sampler2D textureSampler;

out vec4 FragColor;

void main()
{
    vec4 textureColor = texture2D(textureSampler, UVcoords);
    if(textureColor.a < 0.5)
        discard;

    FragColor = textureColor;
}