Texture2D t_colorMap : register(t0);
Texture2D t_normalMap : register(t1);
Texture2D t_skyboxlMap : register(t2);
RWTexture2D<float4> t_outputMap : register(u0);

cbuffer Viewport : register(b0)
{
  float2 ScreenSize;
  float2 unused;
}

[numthreads(32, 32, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  if (dtID.x >= ScreenSize.x || dtID.y >= ScreenSize.y) {
    return;
  }

  float4 normal = t_normalMap.Load(uint3(dtID.xy, 0));
  float len = length(normal.xyz);
  float4 color;
    
  if(len < 0.001f) {
    color = t_skyboxlMap.Load(uint3(dtID.xy, 0));
  }
  else {
    color = t_colorMap.Load(uint3(dtID.xy, 0));
  }
    
  t_outputMap[dtID.xy] = color;
}