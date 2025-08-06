#include "ShaderConstants.hlsl"

Texture2D<float4> t_skybox : register(t0);
RWTexture2D<float4> t_outputMap : register(u0);

cbuffer PrefilterConstants : register(b2)
{
  uint width;
  uint height;
  uint samples;
  float roughness;
  float mipmapLevels;
  float3 pcPadding; // Padding to 16 bytes
};

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
  uint mip = mipmapLevels;
  float3 color = float3(0.0f, 0.0f, 0.0f);
  if (mip == 0)
  {
    color = t_skybox.SampleLevel(samplerLinearClamp, skyUV, mip).xyz;
  }
  else
  {
    float mipFloor = floor(mip);
    float mipFrac = mip - mipFloor;
    float3 color1 = t_skybox.SampleLevel(samplerLinearClamp, skyUV, mipFloor).xyz;
    float3 color2 = t_skybox.SampleLevel(samplerLinearClamp, skyUV, mipFloor + 1).xyz;
    color = lerp(color1, color2, mipFrac);
  }

  t_outputMap[dtID.xy] = float4(color, 1.0f);
}
