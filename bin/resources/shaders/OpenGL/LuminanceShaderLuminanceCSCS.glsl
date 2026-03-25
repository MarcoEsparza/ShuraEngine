#version 430 core

#include "ShaderConstants.glsl"

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(binding = 0) uniform sampler2D t_inputMap;
layout(binding = 0, rgba32f) uniform image2D t_outputMap;

uniform ivec2 inputSize;
uniform ivec2 outputSize;

float Luminance(vec3 color)
{
  return dot(color, vec3(0.2126, 0.7152, 0.0722));
}

void main()
{
  uvec2 dtID = gl_GlobalInvocationID.xy;

  if (dtID.x >= uint(inputSize.x) || dtID.y >= uint(inputSize.y)) {
    return;
  }

  vec2 uv = (vec2(dtID) + 0.5) / vec2(inputSize);
  ivec2 finalID = ivec2(uv * vec2(outputSize));
  vec3 color = texelFetch(t_inputMap, ivec2(dtID), 0).rgb;
  float luminance = Luminance(color);

  imageStore(t_outputMap, finalID, vec4(luminance));
}
