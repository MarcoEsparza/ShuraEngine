#include "ShaderConstants.hlsl"

Texture2D t_skybox : register(t0);
RWTexture2DArray<float4> t_outputMap : register(u0);

#ifndef PI
#define PI 3.14159265359
#endif
#ifndef RECIPROCAL_PI
#define RECIPROCAL_PI 1.0f / 3.14159265359
#endif
#ifndef RECIPROCAL_2PI
#define RECIPROCAL_2PI 1.0f / (2 * 3.14159265359)
#endif

#define CUBEMAP_SIZE 1024

float2
getSkyBoxUV(float3 dir)
{
  float u = -atan2(dir.z, dir.x) * RECIPROCAL_2PI + 0.5f;
  float v = acos(dir.y) * RECIPROCAL_PI;
  return float2(u, v);
}

float3
texCoordToDir(uint faceIndex, float2 uv)
{
  uv = uv * 2.0f - 1.0f; // Map from [0,1] to [-1,1]

  float3 dir = 0;
  switch (faceIndex)
  {
    case 0:
      dir = normalize(float3(1.0, -uv.y, -uv.x));
      break; // +X
    case 1:
      dir = normalize(float3(-1.0, -uv.y, uv.x));
      break; // -X
    case 2:
      dir = normalize(float3(uv.x, 1.0, uv.y));
      break; // +Y
    case 3:
      dir = normalize(float3(uv.x, -1.0, -uv.y));
      break; // -Y
    case 4:
      dir = normalize(float3(uv.x, -uv.y, 1.0));
      break; // +Z
    case 5:
      dir = normalize(float3(-uv.x, -uv.y, -1.0));
      break; // -Z
  }
  return dir;
}

[numthreads(32, 32, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  if(dtID.x >= CUBEMAP_SIZE || dtID.y >= CUBEMAP_SIZE || dtID.z >= 6) {
    return;
  }

  float2 uv = (dtID.xy + 0.5f) / CUBEMAP_SIZE;
  float3 dir = texCoordToDir(dtID.z, uv);
  float2 equirectUV = getSkyBoxUV(dir);
  
  float3 hdrColor = sRGBToLinear(t_skybox.SampleLevel(samplerLinearClamp, equirectUV, 0)).xyz;
  
  t_outputMap[dtID.xyz] = float4(hdrColor, 1.0f);
}
