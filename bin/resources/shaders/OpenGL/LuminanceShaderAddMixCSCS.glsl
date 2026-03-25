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

  ivec2 coord = ivec2(dtID);

  vec4 a = texelFetch(t_inputMap, coord, mipLevel0);

  vec2 uv = (vec2(dtID) + 0.5) / vec2(textureSize);
  vec4 b = textureLod(t_texture1, uv, float(mipLevel1));

  vec4 result = 0.5 * (a + b);

  imageStore(t_outputMap, coord, result);
}
