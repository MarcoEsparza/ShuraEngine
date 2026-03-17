#include "ShaderConstants.hlsl"

Texture2D t_inputMap : register(t0);

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 Texcoord : TEXCOORD0;
};

float4 mainPS(PS_INPUT input) : SV_TARGET
{
  float2 screenUV = input.Position.xy / screenSize;
    
  float4 inputColor = t_inputMap.Sample(samplerLinearWrap, screenUV);
  return inputColor;
}