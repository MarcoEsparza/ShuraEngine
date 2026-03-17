#include "ShaderConstants.hlsl"

Texture2D t_baseColor : register(t0);
Texture2D t_normal : register(t1);
Texture2D t_metallic : register(t2);
Texture2D t_roughness : register(t3);
Texture2D t_ambientO : register(t4);
Texture2D t_emissive : register(t5);
Texture2D t_specular : register(t6);
Texture2D t_opacityMask : register(t7);

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
  float alphaCutoff; // alpha cutoff
  float2 metallicRoughnessFactors; // x = metallic, y = roughness
  int materialBitfield; // bitfield for material properties
  float unused1; // padding to 16 bytes
  float3 emissiveFactor; // emissive factor
  float matEmmissiveIntensity; // emissive intensity
};

struct VS_INPUT
{
  float3 Position : POSITION;
  float3 Normal : NORMAL0;
  float2 Tex : TEXCOORD0;
  float3 Tangent : TANGENT0;
  float3 Bitangent : BINORMAL0;
  float4 BoneIDs : BLENDINDICES0;
  float4 BoneWeights : BLENDWEIGHT0;
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
  float4 Emissive : SV_Target4;
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
    
  /*************************************/
  /*            DIFFUSE MAP            */
  /*************************************/
#if defined(HAS_DIFFUSE_MAP)
  output.Color = t_baseColor.Sample(samplerLinearWrap, input.Tex);
  output.Color = output.Color * float4(baseColorFactor, 1.0f);
#else
  output.Color = float4(baseColorFactor, 1.0f);
#endif

  /*************************************/
  /*         OPACITY MASK MAP          */
  /*************************************/
#if defined(HAS_OPACITY_MASK)
  float opacity = t_opacityMask.Sample(samplerLinearWrap, input.Tex).r;
  output.Color.a = opacity * output.Color.a;
#endif

  /*************************************/
  /*           ALPHA TESTING           */
  /*************************************/
#if defined(HAS_ALPHA_TESTING)
  if (output.Color.a < ALPHA_TEST_THRESHOLD)
  {
    discard;
  }
  //clip(output.Color.a - ALPHA_TEST_THRESHOLD);
#endif
  
  /*************************************/
  /*             NORMAL MAP            */
  /*************************************/
#if defined(HAS_NORMAL_MAP)
  float3 fvNormal = t_normal.Sample(samplerLinearWrap, input.Tex).xyz * 2.0f - 1.0f;
  fvNormal = normalize(mul(fvNormal, float3x3(input.Tangent, input.Bitangent, input.Normal)));
  output.Normal = float4(fvNormal * 0.5f + 0.5f, 1.0f);
#else
  float3 fvNormal = float3(1.0f, 1.0f, 1.0f);
  fvNormal = normalize(mul(fvNormal, float3x3(input.Tangent, input.Bitangent, input.Normal)));
  output.Normal = float4(fvNormal * 0.5f + 0.5f, 1.0f);
#endif

  /*************************************/
  /*               DEPTH               */
  /*************************************/
  output.Depth = float4(input.Depth.xyz, 1.0f);

  /*************************************/
  /*           METALNESS MAP           */
  /*************************************/
#if defined(HAS_METALNESS_MAP)
  output.Properties.b = t_metallic.Sample(samplerLinearWrap, input.Tex).b;
#else
  output.Properties.b = metallicRoughnessFactors.x; // metallic factor
#endif

  /*************************************/
  /*           ROUGHNESS MAP           */
  /*************************************/
#if defined(HAS_ROUGHNESS_MAP)
  output.Properties.g = t_roughness.Sample(samplerLinearWrap, input.Tex).g;
#else
  output.Properties.g = metallicRoughnessFactors.y; // roughness factor
#endif

  /*************************************/
  /*               AO MAP              */
  /*************************************/
#if defined(HAS_AO_MAP)
  output.Properties.r = t_ambientO.Sample(samplerLinearWrap, input.Tex).r;
#else
  output.Properties.r = 1.0f;
#endif
  
  /*************************************/
  /*            EMMISIVE MAP           */
  /*************************************/
#if defined(USE_EMISSION)
 #if defined(HAS_EMISSIVE_MAP)
  output.Emissive = t_emissive.Sample(samplerLinearWrap, input.Tex);
 #else
  output.Emissive = float4(emissiveFactor, 1.0f);
 #endif
#else
  output.Emissive = float4(0.0f, 0.0f, 0.0f, 0.0f);
#endif
  
  /*************************************/
  /*           INVERT NORMALS          */
  /*************************************/
  
  /*************************************/
  /*          INVERT ROUGHNESS         */
  /*************************************/
#if defined(INVERT_ROUGHNESS)
  output.Properties.b = 1.0f - output.Properties.b;
#endif
  
  /*************************************/
  /*          INVERT METALNESS         */
  /*************************************/
  
  return output;
}