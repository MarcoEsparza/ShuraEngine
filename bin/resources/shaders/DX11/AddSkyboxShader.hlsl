#include "ShaderConstants.hlsl"

Texture2D t_colorMap : register(t0);
Texture2D t_depthMap : register(t1);
Texture2D t_skyboxMap : register(t2);
RWTexture2D<float4> t_outputMap : register(u0);

[numthreads(32, 32, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  if (dtID.x >= screenSize.x || dtID.y >= screenSize.y) {
    return;
  }

  //float2 uv = (dtID.xy + 0.5f) / screenSize;

  float depth = t_depthMap.Load(uint3(dtID.xy, 0));
  float4 color;
    
  if (depth >= 0.9999f)
  {
    color = t_skyboxMap.Load(uint3(dtID.xy, 0));
  }
  else {
    color = t_colorMap.Load(uint3(dtID.xy, 0));
  }
    
  t_outputMap[dtID.xy] = color;
}