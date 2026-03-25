#include "ShaderConstants.hlsl"

Texture2D<float4> t_skybox : register(t0);
RWTexture2D<float4> t_outputMap : register(u0);

[numthreads(32, 32, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  float2 ndc = dtID.xy / screenSize * 2.0f - 1.0f;
  ndc.y = -ndc.y;
  float4 clipSpacePos = float4(ndc, 1.0f, 1.0f);
  
  matrix newViewMatrix = matView;
  newViewMatrix[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
  
  float4 pos = mul(clipSpacePos, newViewMatrix);
  pos = float4(pos.xyz, 1.0f);
  
  pos = mul(pos, matProjectionTranspose);
  pos.z = pos.w;
  
  float3 dir = normalize(pos.xyz);
  float2 skyUV = getSkyBoxUV(dir);
  float3 color = float3(0.0f, 0.0f, 0.0f);
  
  color = t_skybox.SampleLevel(samplerLinearClamp, skyUV, 0).xyz;

  t_outputMap[dtID.xy] = float4(color, 1.0f);
}
