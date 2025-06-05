Texture2D<float4> t_toneMap : register(t0);
Texture2D<float4> t_blurBloom: register(t1);
RWTexture2D<float4> t_output : register(u0);

cbuffer Viewport : register(b0)
{
  float2 Dimensions;
  float unused;
  float unused1;
}

[numthreads(32, 32, 1)]
void
CSMain( uint3 dtID : SV_DispatchThreadID )
{
  if (dtID.x >= Dimensions.x || dtID.y >= Dimensions.y) {
    return;
  }
    
  float4 color = t_toneMap.Load(uint3(dtID.xy, 0));
  float4 bloom = t_blurBloom.Load(uint3(dtID.xy, 0));
    
  //float4 finalColor = color + bloom;
  //finalColor = saturate(finalColor);
    
  float4 finalColor = float4(0.5f * (color.rgb + bloom.rgb), 1.0f);
    
  t_output[dtID.xy] = finalColor;
}