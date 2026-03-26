#version 430 core

#include "ShaderConstants.glsl"

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(binding = 0) uniform sampler2D t_inputMap;
layout(binding = 1) uniform sampler2D t_texture1;

layout(binding = 0, rgba32f) uniform image2D t_outputMap;

void main()
{
  uvec2 id = gl_GlobalInvocationID.xy;

  if (id.x >= uint(screenSize.x) || id.y >= uint(screenSize.y)) {
    return;
  }

  ivec2 coord = ivec2(id);
  vec3 color = texelFetch(t_inputMap, coord, 0).rgb;
  vec3 emissive = texelFetch(t_texture1, coord, 0).rgb;
  vec3 finalColor = color + emissive * emmisiveIntensity;

  imageStore(t_outputMap, coord, vec4(finalColor, 1.0));
}
