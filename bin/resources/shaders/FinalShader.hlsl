#include "resources/shaders/ShaderConstants.hlsl"

SamplerState textureSampler : register(s0);
//Texture2D t_normalMap : register(t0);
//Texture2D t_skybox : register(t1);
//Texture2D t_colorMap : register(t2);
Texture2D t_inputMap : register(t0);

//cbuffer Viewport : register(b0)
//{
//  float2 Dimensions;
//  float2 unused;
//}

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 Texcoord : TEXCOORD0;
};

//float4 getNormal(in float2 uv)
//{
//  float4 normal = t_normalMap.Sample(textureSampler, uv);
//  normal.xyz = normal.xyz * 2.0f - 1.0f;
//  return normal;
//}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
  float2 screenUV = input.Position.xy / screenSize;
   
  //float4 color = t_colorMap.Sample(textureSampler, screenUV);
  //float4 normal = t_normalMap.Sample(textureSampler, screenUV);
  
  //if(length(normal.xyz) > 0.01f)
  //{
  //  return color;
  //}
    
  //float4 skyBoxColor = t_skybox.Sample(textureSampler, screenUV);
    
  //return skyBoxColor;
    
  float4 inputColor = t_inputMap.Sample(textureSampler, screenUV);
  return inputColor;
}