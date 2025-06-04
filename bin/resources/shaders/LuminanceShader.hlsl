Texture2D<float4> t_inputMap : register(t0);
RWTexture2D<float4> t_outputMap : register(u0);

cbuffer Viewport : register(b0)
{
  float2 Dimensions;
  float unused;
  float unused1;
}

cbuffer BrightParams : register(b1)
{
  float threshold;
  float3 unused2;
}

float
CalLuminance(float3 color)
{
  return dot(color, float3(0.2126, 0.7152, 0.0722));
}

[numthreads(32, 32, 1)]
void
CSMain( uint3 dtID : SV_DispatchThreadID )
{
  if (dtID.x >= Dimensions.x || dtID.y >= Dimensions.y) {
    return;
  }
    
  float4 color = t_inputMap.Load(int3(dtID.xy, 0));
  float luminance = CalLuminance(color.rgb);
  //float4 result = (luminance > threshold) ? color : float4(0.0f, 0.0f, 0.0f, 0.0f);
    
  float3 bloomColor = max(color - threshold, 0.0f);
  //bloomColor
    
  t_outputMap[dtID.xy] = bloomColor;
}