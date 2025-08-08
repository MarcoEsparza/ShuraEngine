#include "ShaderConstants.hlsl"

Texture2D<float4> t_inputMap : register(t0);
Texture2D<float4> t_texture1 : register(t1);
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
  uint2 dimensions;
  t_inputMap.GetDimensions(dimensions.x, dimensions.y);
  if (dtID.x >= dimensions.x || dtID.y >= dimensions.y) {
    return;
  }
  
  uint2 outputDimensions;
  t_outputMap.GetDimensions(outputDimensions.x, outputDimensions.y);
  float2 uv = (float2(dtID.x, dtID.y) + 0.5f) /
              float2(dimensions.x, dimensions.y);
  uint2 finalID = uint2(uv * float2(outputDimensions.x, outputDimensions.y));
  float4 color = t_inputMap.Load(int3(dtID.xy, 0));
  float luminance = Luminance(color.rgb);
  //t_outputMap[dtID.xy] = log(max(luminance, 0.0001f));
  t_outputMap[finalID] = luminance;
}

[numthreads(32, 32, 1)]
void
BrightCS( uint3 dtID : SV_DispatchThreadID )
{
  uint2 dimensions;
  t_outputMap.GetDimensions(dimensions.x, dimensions.y);
  if (dtID.x >= dimensions.x || dtID.y >= dimensions.y) {
    return;
  }
  
  // Sample the color texture
  float2 uv = (float2(dtID.x, dtID.y) + 0.5f) / float2(dimensions.x, dimensions.y);
  //uint2 colorDimensions;
  //t_inputMap.GetDimensions(colorDimensions.x, colorDimensions.y);
  //uint2 colorUV = uint2(uv * float2(colorDimensions.x, colorDimensions.y));
  
  // Sample the luminance texture
  //uint2 luminanceDimensions;
  //t_texture1.GetDimensions(luminanceDimensions.x, luminanceDimensions.y);
  //uint2 luminanceUV = uint2(uv * float2(luminanceDimensions.x, luminanceDimensions.y));
  
  //float4 color = t_inputMap.Load(int3(colorUV, 0));
  //float luminance = t_texture1.Load(uint3(luminanceUV, 0)).r;
  
  float3 color = t_inputMap.SampleLevel(samplerLinearClamp, uv, 0).rgb;
  float luminance = t_texture1.SampleLevel(samplerLinearClamp, uv, 0).r;
  
  //t_outputMap[dtID.xy] = float4(luminance.xxx, 1.0f);
  //return;
  
  float3 brightColor = max(color - brightThreshold, 0.0f);
  brightColor *= step(brightThreshold, luminance);
    
  t_outputMap[dtID.xy] = float4(brightColor, 1.0f);
}

[numthreads(32, 32, 1)]
void
AddMixCS( uint3 dtID : SV_DispatchThreadID )
{
  uint2 dimensions;
  t_outputMap.GetDimensions(dimensions.x, dimensions.y);
  //if (dtID.x >= dimensions.x || dtID.y >= dimensions.y) {
  //  return;
  //}
  
  //t_outputMap[dtID.xy] = float4(0.5f * (t_inputMap.Load(int3(dtID.xy, 0)) +
  //                                      t_texture1.Load(uint3(dtID.xy, 0))));
  
  t_outputMap[dtID.xy] = 0.5f * (t_inputMap.Load(float3(dtID.xy, mipLevel0)) +
                         t_texture1.SampleLevel(samplerLinearClamp, (dtID.xy + 0.5f) /
                         float2(dimensions.x, dimensions.y), mipLevel1));
}