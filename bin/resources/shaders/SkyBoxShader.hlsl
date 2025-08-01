#include "ShaderConstants.hlsl"

Texture2D t_skybox : register(t0);
RWTexture2D<float4> t_outputMap : register(u0);

#ifndef PI
#define PI 3.14159265359
#endif
#ifndef RECIPROCAL_PI
#define RECIPROCAL_PI 1.0f / 3.14159265359
#endif
#ifndef RECIPROCAL_2PI
#define RECIPROCAL_2PI 1.0f / (2 * 3.14159265359)
#endif

float2
getSkyBoxUV(float3 dir)
{
  float u = -atan2(dir.z, dir.x) * RECIPROCAL_2PI + 0.5f;
  float v = acos(dir.y) * RECIPROCAL_PI;
  return float2(u, v);
}


float2
EquirectUV(float3 dir)
{
  float u = atan2(dir.z, dir.x) * RECIPROCAL_2PI + 0.5;
  float v = acos(clamp(dir.y, -1.0, 1.0)) * RECIPROCAL_PI;
  return float2(u, v);
}

[numthreads(32, 32, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  float2 ndc = dtID.xy / screenSize * 2.0f - 1.0f;
  ndc.y = -ndc.y;
  float4 clipSpacePos = float4(ndc, 1.0f, 1.0f);

  matrix newViewMatrix = matViewTranspose;
  newViewMatrix[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);

  float4 pos = mul(clipSpacePos, newViewMatrix);
  pos = float4(pos.xyz, 1.0f);

  pos = mul(pos, matProjectionTranspose);
  pos.z = pos.w;

  float3 dir = normalize(pos.xyz);
  float2 skyUV = getSkyBoxUV(dir);
  float3 color = sRGBToLinear(t_skybox.SampleLevel(samplerLinearClamp, skyUV, 0)).xyz;

  t_outputMap[dtID.xy] = float4(color, 1.0f);
}
