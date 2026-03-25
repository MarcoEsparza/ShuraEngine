#version 430 core

#include "ShaderConstants.glsl"

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(binding = 0) uniform sampler2D t_skybox;
layout(binding = 0, rgba32f) uniform image2D t_outputMap;

void main()
{
  uvec2 id = gl_GlobalInvocationID.xy;

  if (id.x >= uint(screenSize.x) || id.y >= uint(screenSize.y)) {
    return;
  }

  vec2 ndc = (vec2(id) / screenSize) * 2.0 - 1.0;
  ndc.y = -ndc.y;

  vec4 clipSpacePos = vec4(ndc, 1.0, 1.0);

  mat4 view = matView;
  view[3] = vec4(0.0, 0.0, 0.0, 1.0);

  vec4 pos = view * clipSpacePos;
  pos = vec4(pos.xyz, 1.0);
  pos = matProjection * pos;
  pos.z = pos.w;

  vec3 dir = normalize(pos.xyz);
  vec2 skyUV = getSkyBoxUV(dir);
  vec3 color = textureLod(t_skybox, skyUV, 0.0).rgb;

  imageStore(t_outputMap, ivec2(id), vec4(color, 1.0));
}
