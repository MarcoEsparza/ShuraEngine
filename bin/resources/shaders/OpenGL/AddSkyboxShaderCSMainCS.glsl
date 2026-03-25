#version 430 core

#include "ShaderConstants.glsl"

layout (local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout (binding = 0) uniform sampler2D t_colorMap;
layout (binding = 1) uniform sampler2D t_depthMap;
layout (binding = 2) uniform sampler2D t_skyboxMap;

layout (binding = 0, rgba32f) uniform image2D t_outputMap;

void main()
{
  uvec3 dtID = gl_GlobalInvocationID;

  if (dtID.x >= uScreenSize.x || dtID.y >= uScreenSize.y) 
  {
    return;
  }

  ivec2 coord = ivec2(dtID.xy);

  float depth = texelFetch(t_depthMap, coord, 0).r;
  vec4 color;

  if (depth == 1.0)
  {
    color = texelFetch(t_skyboxMap, coord, 0);
  }
  else
  {
    color = texelFetch(t_colorMap, coord, 0);
  }

  imageStore(t_outputMap, coord, color);
}
