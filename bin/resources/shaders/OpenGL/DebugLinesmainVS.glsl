#version 430 core

#include "ShaderConstants.glsl"

layout(location = 0) in vec3 aPosition;

void main()
{
    gl_Position = matProjection * matView * vec4(aPosition, 1.0);
}