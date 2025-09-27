#include "ShaderConstants.hlsl"

//cbuffer VP : register(b3)
//{
//  float4x4 View;
//  float4x4 Proj;
//}

cbuffer Model : register(b2)
{
  float4x4 ModelTransform;
}

cbuffer LightData : register(b3)
{
  float4 LightPos;
  float3 lightTarget;
  float lightIntensity;
  float4 lightColor;
  float4x4 lightView;
  float4x4 lightProj;
}

struct VS_INPUT
{
  float3 Position : POSITION;
};

struct PS_INPUT
{
  float4 Position : SV_POSITION;
};

PS_INPUT main(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT) 0;
  
  float4x4 wvp = mul(ModelTransform, mul(lightView, lightProj));
  output.Position = mul(float4(input.Position.xyz, 1.0f), wvp);
  
  return output;
}

float4 mainPS(PS_INPUT input) : SV_Target
{
  float depth = input.Position.z / input.Position.w;
  return float4(depth.xxx, 1.0f);
}