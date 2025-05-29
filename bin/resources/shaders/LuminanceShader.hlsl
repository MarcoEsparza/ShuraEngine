SamplerState textureSampler : register(s0);
Texture2D<float4> t_inputMap : register(t0);
RWTexture2D<float4> t_outputMap : register(u0);

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
    
  float2 coord = float2(dtID.x, dtID.y) / float2(Dimensions.x, Dimensions.y);
  float4 color = t_inputMap.Load(int3(dtID.xy, 0));
  
  float luminance = dot(color.rgb, float3(0.2126, 0.7152, 0.0722));
    
  t_outputMap[dtID.xy] = float4(luminance, luminance, luminance, 1.0f);
}