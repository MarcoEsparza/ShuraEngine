#include "resources/shaders/ShaderConstants.hlsl"

Texture2D<float4> t_inputMap : register(t0);
Texture2D<float4> t_blurBloom : register(t1);
RWTexture2D<float4> t_outputMap : register(u0);

float
Luminance(float3 color)
{
  return dot(color, float3(0.2126f, 0.7152f, 0.0722f));
}

[numthreads(32, 32, 1)]
void
LuminanceCS( uint3 dtID : SV_DispatchThreadID )
{
  if (dtID.x >= screenSize.x || dtID.y >= screenSize.y) {
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
  if (dtID.x >= screenSize.x || dtID.y >= screenSize.y) {
    return;
  }
    
  float4 color = t_inputMap.Load(int3(dtID.xy, 0));
  float luminance = Luminance(color.rgb);
    
  float3 bloomColor = max(color - brightThreshold, 0.0f);
  bloomColor *= step(brightThreshold, luminance);
    
  t_outputMap[dtID.xy] = float4(bloomColor, 0.0f);
}

[numthreads(32, 32, 1)]
void
AddMixCS( uint3 dtID : SV_DispatchThreadID )
{
  if (dtID.x >= screenSize.x || dtID.y >= screenSize.y) {
    return;
  }
    
  float4 color = t_inputMap.Load(uint3(dtID.xy, 0));
  float4 bloom = t_blurBloom.Load(uint3(dtID.xy, 0));
    
  float4 finalColor = float4(0.5f * (color.rgb + bloom.rgb), 1.0f);
    
  t_outputMap[dtID.xy] = finalColor;
}