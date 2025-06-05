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
  float bloomThreshold;
  float3 unused2;
}

float
Luminance(float3 color)
{
  return dot(color, float3(0.2126f, 0.7152f, 0.0722f));
}

[numthreads(32, 32, 1)]
void
LuminanceCS( uint3 dtID : SV_DispatchThreadID )
{
  if (dtID.x >= Dimensions.x || dtID.y >= Dimensions.y) {
    return;
  }
    
  float4 color = t_inputMap.Load(int3(dtID.xy, 0));
  float luminance = Luminance(color.rgb);
  t_outputMap[dtID.xy] = log(max(luminance, 0.0001f));
}

[numthreads(32, 32, 1)]
void
BrightCS( uint3 dtID : SV_DispatchThreadID )
{
  if (dtID.x >= Dimensions.x || dtID.y >= Dimensions.y) {
    return;
  }
    
  float4 color = t_inputMap.Load(int3(dtID.xy, 0));
  float luminance = Luminance(color.rgb);
    
  float3 bloomColor = max(color - bloomThreshold, 0.0f);
  bloomColor *= step(bloomThreshold, luminance);
    
  t_outputMap[dtID.xy] = float4(bloomColor, 0.0f);
}