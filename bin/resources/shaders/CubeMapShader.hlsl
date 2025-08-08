#include "ShaderConstants.hlsl"

Texture2D t_skybox : register(t0);
RWTexture2DArray<float4> t_outputMap : register(u0);

#define CUBEMAP_SIZE 1024

//float2
//getSkyBoxUV(float3 dir)
//{
//  float u = -atan2(dir.z, dir.x) * RECIPROCAL_2PI + 0.5f;
//  float v = acos(dir.y) * RECIPROCAL_PI;
//  return float2(u, v);
//}

float2
EquirectUV(float3 dir)
{
  float u = atan2(dir.z, dir.x) * RECIPROCAL_2PI + 0.5;
  float v = acos(clamp(dir.y, -1.0, 1.0)) * RECIPROCAL_PI;
  return float2(u, v);
}

float3
texCoordToDir(uint faceIndex, float2 uv)
{
  uv = uv * 2.0f - 1.0f; // Map from [0,1] to [-1,1]

  float3 dir = 0;
  if(faceIndex == 0) {
    dir = float3(1.0, -uv.y, -uv.x);
  }
  else if(faceIndex == 1) {
    dir = float3(-1.0, -uv.y, uv.x);
  }
  else if(faceIndex == 2) {
    dir = float3(uv.x, 1.0, uv.y);
  }
  else if(faceIndex == 3) {
    dir = float3(uv.x, -1.0, -uv.y);
  }
  else if(faceIndex == 4) {
    dir = float3(uv.x, -uv.y, 1.0);
  }
  else if(faceIndex == 5) {
    dir = float3(-uv.x, -uv.y, -1.0);
  }
  
  return normalize(dir);
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
