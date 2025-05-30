Texture2D t_inputMap : register(t0);
RWTexture2D<float4> t_outputMap : register(u0);

cbuffer Viewport : register(b0)
{
  float2 Dimensions;
  float unused;
  float unused1;
}

cbuffer Limits : register(b1)
{
  float minR;
  float maxR;
  float minG;
  float maxG;
  float minB;
  float maxB;
  float2 unused2;
}

[numthreads(32, 32, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  if (dtID.x >= Dimensions.x || dtID.y >= Dimensions.y) {
    return;
  }

  float4 color = t_inputMap.Load(uint3(dtID.xy, 0));
  color = saturate(color);
    
  color.r = lerp(minR, maxR, color.r);
  color.g = lerp(minG, maxG, color.g);
  color.b = lerp(minB, maxB, color.b);
    
  t_outputMap[dtID.xy] = color;
}