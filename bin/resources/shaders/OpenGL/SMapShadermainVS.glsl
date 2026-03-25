#version 430

#include "ShaderConstants.glsl"

layout(location = 0) in vec3 aPosition;

layout(std140, binding = 2) uniform Model
{
    mat4 ModelTransform;
};

layout(std140, binding = 3) uniform LightData
{
    vec4 LightPos;
    vec3 lightTarget;
    float lightIntensity;
    vec4 lightColor;
    mat4 lightView;
    mat4 lightProj;
};

void main()
{
  mat4 wvp = lightProj * lightView * ModelTransform;
  gl_Position = wvp * vec4(aPosition, 1.0);
}
