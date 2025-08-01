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

[numthreads(32, 32, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  

  t_outputMap[dtID.xy] = float4(0.0f, 0.0f, 0.0f, 1.0f);
}
