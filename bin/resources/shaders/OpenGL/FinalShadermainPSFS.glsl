#version 430 core

#include "ShaderConstants.glsl"

in vec2 Texcoord;
out vec4 FragColor;

layout(binding = 0) uniform sampler2D t_inputMap;

void main()
{
  vec4 inputColor = texture(t_inputMap, Texcoord);
  FragColor = inputColor;
}
