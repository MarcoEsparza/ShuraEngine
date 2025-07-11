#include "ShaderConstants.hlsl"

SamplerState textureSampler : register(s0);
Texture2D t_inputMap : register(t0);

//cbuffer VP : register(b0)
//{
//  float4x4 viewMat;
//  float4x4 projMat;
//}

//cbuffer Model : register(b3)
//{
//  float4x4 transformMat;
//}

struct VS_INPUT
{
  float3 position : POSITION;
};

struct PS_INPUT
{
  float4 position : SV_Position;
};

PS_INPUT main(VS_INPUT input)
{
  PS_INPUT output;
  output.position = mul(float4(input.position, 1.0f),
                    mul(matViewTranspose, matProjectionTranspose));
  return output;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
  return float4(1.0f, 1.0f, 0.0f, 1.0f);
}