SamplerState textureSampler : register(s0);
Texture2D t_baseColor : register(t0);
Texture2D t_normal : register(t1);
Texture2D t_metallic : register(t2);
Texture2D t_roughness : register(t3);
Texture2D t_ambientO : register(t4);

cbuffer VP : register(b0)
{
  float4x4 View;
  float4x4 Proj;
}

cbuffer Model : register(b1)
{
  float4x4 ModelTransform;
}

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
  float4 Depth : COLOR0;
  float4 Normal : COLOR1;
  float4 Color : COLOR2;
  float4 Properties : COLOR2;
};

PS_INPUT main(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT) 0;
  
  float4x4 wvp = mul(ModelTransform, mul(View, Proj));

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
    
  output.Color = t_baseColor.Sample(textureSampler, input.Tex);
  float3 fvNormal = t_normal.Sample(textureSampler, input.Tex).xyz * 2.0f - 1.0f; 
  fvNormal = normalize(mul(fvNormal, float3x3(input.Tangent, input.Bitangent, input.Normal)));
  output.Normal = float4(fvNormal * 0.5f + 0.5f, 1.0f);
  output.Depth = float4(input.Depth.xyz, 1.0f);
  //output.Color.a = t_metallic.Sample(textureSampler, input.Tex);
  //output.Normal.a = t_roughness.Sample(textureSampler, input.Tex);
  output.Properties.r = t_metallic.Sample(textureSampler, input.Tex);
  output.Properties.b = t_roughness.Sample(textureSampler, input.Tex);
  
  return output;
}