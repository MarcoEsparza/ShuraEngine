#version 330 core

// Values from vertex shaders
in vec3 Normal;
in vec2 UV;

// Output data
out vec3 color;

// Values constant
uniform sampler2D myTextureSampler;

void main()
{
    color = texture(myTextureSampler, UV).rgb;
}