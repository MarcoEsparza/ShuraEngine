SamplerState textureSampler : register(s0);
Texture2D t_normalMap : register(t0);
Texture2D t_skybox : register(t1);

cbuffer Viewport : register(b0)
{
  float2 Dimensions;
  float2 unused;
}

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 Texcoord : TEXCOORD0;
};

float4 getNormal(in float2 uv)
{
  float4 normal = t_normalMap.Sample(textureSampler, uv);
  normal.xyz = normal.xyz * 2.0f - 1.0f;
  return normal;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
  float2 screenUV = input.Position.xy / Dimensions;
   
  float4 normal = t_normalMap.Sample(textureSampler, screenUV);
  //float4 normal = getNormal(screenUV);
  if(length(normal.xyz) > 0.01f)
  {
    discard;
  }
    
  float4 skyBoxColor = t_skybox.Sample(textureSampler, screenUV);
    
  return skyBoxColor;
}