#version 430 core

#include "ShaderConstants.glsl"

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(binding = 0) uniform sampler2D t_inputMap;
layout(binding = 1) uniform sampler2D t_texture1;

layout(binding = 0, rgba32f) uniform image2D t_outputMap;

uniform ivec2 textureSize;
uniform ivec2 outputSize;

void main()
{
  uvec2 dtID = gl_GlobalInvocationID.xy;

  if (dtID.x >= uint(textureSize.x) || dtID.y >= uint(textureSize.y)) {
    return;
  }

  vec2 uv = (vec2(dtID) + 0.5) / vec2(textureSize);

  vec3 color = textureLod(t_inputMap, uv, 0.0).rgb;
  float luminance = textureLod(t_texture1, uv, 0.0).r;

  vec3 brightColor = max(color - vec3(brightThreshold), vec3(0.0));
  brightColor *= step(brightThreshold, luminance);

  imageStore(t_outputMap, ivec2(dtID), vec4(brightColor, 1.0));
}
