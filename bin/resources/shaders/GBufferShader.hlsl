#include "ShaderConstants.hlsl"

//SamplerState textureSampler : register(s0);
Texture2D t_baseColor : register(t0);
Texture2D t_normal : register(t1);
Texture2D t_metallic : register(t2);
Texture2D t_roughness : register(t3);
Texture2D t_ambientO : register(t4);

//cbuffer VP : register(b0)
//{
//  float4x4 View;
//  float4x4 Proj;
//}

#ifndef ALPHA_TEST_THRESHOLD
#define ALPHA_TEST_THRESHOLD 0.5f
#endif

cbuffer Model : register(b2)
{
  float4x4 ModelTransform;
}

cbuffer MaterialData : register(b3)
{
  float3 baseColorFactor; // RGB base color factor
  float unused0; // padding to 16 bytes
  float2 metallicRoughnessFactors; // x = metallic, y = roughness
  int materialBitfield; // bitfield for material properties
  float unused1; // padding to 16 bytes
};

struct VS_INPUT
{
  float3 Position : POSITION;
  float3 Normal : NORMAL0;
  float2 Tex : TEXCOORD0;
  float3 Tangent : TANGENT0;
  float3 Bitangent : BINORMAL0;
};

struct PS_INPUT
{
  float4 Position : SV_POSITION;
  float2 Tex : TEXCOORD0;
  float3 Normal : TEXCOORD1;
  float3 Tangent : TEXCOORD2;
  float3 Bitangent : TEXCOORD3;
  float3 Depth : TEXCOORD4;
};

struct GBUFFER_OUTPUT
{
  float4 Depth : SV_Target0;
  float4 Normal : SV_Target1;
  float4 Color : SV_Target2;
  float4 Properties : SV_Target3;
};

PS_INPUT main(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT) 0;
  
  float4x4 wvp = mul(ModelTransform, mul(matViewTranspose, matProjectionTranspose));
  //float4x4 wvp = mul(ModelTransform, mul(View, Proj));

  output.Position = mul(float4(input.Position.xyz, 1.0f), wvp);
  output.Tex = input.Tex;
  output.Depth = mul(float4(input.Position.xyz, 1.0f), ModelTransform);
    
  output.Normal = normalize(mul(input.Normal, (float3x3)ModelTransform));
  output.Tangent = normalize(mul(input.Tangent, (float3x3)ModelTransform));
  output.Bitangent = normalize(mul(input.Bitangent, (float3x3)ModelTransform));
    
  return output;
}

GBUFFER_OUTPUT mainPS(PS_INPUT input) : SV_Target
{
  GBUFFER_OUTPUT output = (GBUFFER_OUTPUT)0;
    
  MaterialProperties materialProps = getMaterialProperties(materialBitfield);
    
  if (!materialProps.bHasBaseColor)
  {
    output.Color = float4(baseColorFactor, 1.0f);
  }
  else
  {
    output.Color = t_baseColor.Sample(samplerLinearWrap, input.Tex);
    output.Color = output.Color * float4(baseColorFactor, 1.0f);
  }
    
  if(materialProps.bHasAlphaTest)
  {
    if (output.Color.a < ALPHA_TEST_THRESHOLD)
    {
      discard;
    }
  }

  //float3 fvNormal = float3(0.0f, 0.0f, 0.0f);
  if (materialProps.bHasNormalMap)
  {
    float3 fvNormal = t_normal.Sample(samplerLinearWrap, input.Tex).xyz * 2.0f - 1.0f;
    fvNormal = normalize(mul(fvNormal, float3x3(input.Tangent, input.Bitangent, input.Normal)));
    output.Normal = float4(fvNormal * 0.5f + 0.5f, 1.0f);
  }
  else
  {
    //float3 fvNormal = normalize(input.Normal);
    float3 fvNormal = float3(1.0f, 1.0f, 1.0f);
    fvNormal = normalize(mul(fvNormal, float3x3(input.Tangent, input.Bitangent, input.Normal)));
    output.Normal = float4(fvNormal * 0.5f + 0.5f, 1.0f);
  }
    
  output.Depth = float4(input.Depth.xyz, 1.0f);
    
  if (materialProps.bHasMetallicMap)
  {
    output.Properties.r = t_metallic.Sample(samplerLinearWrap, input.Tex).r;
  }
  else
  {
    output.Properties.r = metallicRoughnessFactors.x; // metallic factor
  }
  
  if(materialProps.bHasRoughnessMap)
  {
    output.Properties.b = t_roughness.Sample(samplerLinearWrap, input.Tex).g;
    
  }
  else
  {
    output.Properties.b = metallicRoughnessFactors.y; // roughness factor
  }
  
  if (materialProps.bInvertRoughness)
  {
    output.Properties.b = 1.0f - output.Properties.b;
  }
  
  return output;
}