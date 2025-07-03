#include "resources/shaders/ShaderConstants.hlsl"

SamplerState samplerLinear : register(s0);
SamplerState samplerClamp : register(s1);
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
  uint2 inputDimensions;
  t_inputMap.GetDimensions(inputDimensions.x, inputDimensions.y);
  uint2 inputID = uint2(uv * float2(inputDimensions.x, inputDimensions.y));
  
  // Sample the luminance texture
  uint2 luminanceDimensions;
  t_texture1.GetDimensions(luminanceDimensions.x, luminanceDimensions.y);
  uint2 luminanceID = uint2(uv * float2(luminanceDimensions.x, luminanceDimensions.y));
  
  float4 color = t_inputMap.Load(int3(inputID, 0));
  float luminance = t_texture1.Load(uint3(luminanceID, 0)).r;
    
  float3 brightColor = max(color - brightThreshold, 0.0f);
  brightColor *= step(brightThreshold, luminance);
    
  t_outputMap[dtID.xy] = float4(brightColor, 1.0f);
}

[numthreads(32, 32, 1)]
void
AddMixCS( uint3 dtID : SV_DispatchThreadID )
{
  if (dtID.x >= screenSize.x || dtID.y >= screenSize.y) {
    return;
  }
    
  t_outputMap[dtID.xy] = float4(0.5f * (t_inputMap.SampleLevel(samplerClamp, dtID.xy, mipLevel0).rgb +
                                       t_texture1.SampleLevel(samplerClamp, dtID.xy, mipLevel1).rgb), 1.0f);
    
  //float4 color = t_inputMap.Load(uint3(dtID.xy, 0));
  //float4 bloom = t_texture1.Load(uint3(dtID.xy, 0));
  //  
  //float4 finalColor = float4(0.5f * (color.rgb + bloom.rgb), 1.0f);
  //  
  //t_outputMap[dtID.xy] = finalColor;
}