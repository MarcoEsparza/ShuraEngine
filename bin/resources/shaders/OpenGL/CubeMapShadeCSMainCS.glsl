#version 430

#include "ShaderConstants.glsl"

layout (local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

#define CUBE_MAP_SIZE 1024

layout (binding = 0) uniform sampler2D t_skybox;

layout (binding = 0, rgba32f) uniform image2DArray t_outputMap;

vec2 equirrectUV(vec3 dir)
{
  float u = atan(dir.z, dir.x) / RECIPROCAL_2PI + 0.5;
  float v = acos(clamp(dir.y, -1.0, 1.0)) * RECIPROCAL_PI;
  return vec2(u, v);
}

vec3 texcoordToDir(uint faceIndex, vec2 uv)
{
  uv = uv * 2.0 - 1.0;
  if (faceIndex == 0u) {
    dir = vec3( 1.0, -uv.y, -uv.x);
  }
  else if (faceIndex == 1u) {
    dir = vec3(-1.0, -uv.y,  uv.x);
  }
  else if (faceIndex == 2u) {
    dir = vec3( uv.x,  1.0,  uv.y);
  }
  else if (faceIndex == 3u) {
    dir = vec3( uv.x, -1.0, -uv.y);
  }
  else if (faceIndex == 4u) {
    dir = vec3( uv.x, -uv.y,  1.0);
  }
  else if (faceIndex == 5u) {
    dir = vec3(-uv.x, -uv.y, -1.0);
  }

  return normalize(dir);
}

void main()
{
  uvec3 dtID = gl_GlobalInvocationID;

  if (dtID.x >= CUBEMAP_SIZE || dtID.y >= CUBEMAP_SIZE || dtID.z >= 6u) {
    return;
  }

  vec2 uv = (vec2(dtID.xy) + 0.5) / float(CUBEMAP_SIZE);
  vec3 dir = texCoordToDir(dtID.z, uv);

  vec2 equirectUV = equirrectUV(dir);
  vec3 hdrColor = sRGBToLinear(textureLod(t_skybox, equirectUV, 0.0)).rgb;

  imageStore(t_outputMap, ivec3(dtID.xy, dtID.z), vec4(hdrColor, 1.0));
}
