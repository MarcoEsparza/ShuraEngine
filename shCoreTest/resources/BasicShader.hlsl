SamplerState textureSampler : register(s0);
Texture2D textureData : register(t0);

cbuffer WVP : register(b0)
{
  float4x4 View;
  float4x4 Proj;
}

//cbuffer BoneTransform : register(b1)
//{
//  float4x4 Transform[100];
//}

struct VS_INPUT
{
  float3 Position : POSITION;
  float3 Normal : NORMAL0;
  float2 Tex : TEXCOORD0;
  float4 BoneId : BLENDINDICES0;
  float4 BoneWeights : BLENDWEIGHT0;
};

struct PS_INPUT
{
  float4 Position : SV_POSITION;
  float3 Normal : NORMAL0;
  float2 Tex : TEXCOORD0;
};

PS_INPUT main(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT) 0;
  
  //float4x4 boneTransform = mul(input.BoneWeights.x, Transform[input.BoneId.x]);
  //boneTransform += mul(input.BoneWeights.y, Transform[input.BoneId.y]);
  //boneTransform += mul(input.BoneWeights.z, Transform[input.BoneId.z]);
  //boneTransform += mul(input.BoneWeights.w, Transform[input.BoneId.w]);
  
  //float4 finalVertexPos = mul(float4(input.Position.xyz, 1.0f), boneTransform);
  //output.Position = mul(finalVertexPos, mul(View, Proj));
  
  output.Position = mul(float4(input.Position.xyz, 1.0f), mul(View, Proj));
  output.Normal = input.Normal;
  output.Tex = input.Tex;
  
  return output;
}

float4 mainPS(PS_INPUT input) : SV_Target
{
  return textureData.Sample(textureSampler, input.Tex);
}