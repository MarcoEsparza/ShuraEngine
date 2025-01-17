SamplerState textureSampler : register(s0);
Texture2D textureData : register(t0);

cbuffer VP : register(b0)
{
  float4x4 View;
  float4x4 Proj;
}

cbuffer Model : register(b1)
{
  float4x4 modelPos;
}

struct VS_INPUT
{
  float3 Position : POSITION;
  float3 Normal : NORMAL0;
  float2 Tex : TEXCOORD0;
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
  
  float4 worldPos = mul(float4(input.Position, 1.0), modelPos);
  float4 viewPos = mul(worldPos, View);
  output.Position = mul(viewPos, Proj);
  output.Normal = input.Normal;
  output.Tex = input.Tex;
  
  return output;
}

float4 mainPS(PS_INPUT input) : SV_Target
{
  float4 color = textureData.Sample(textureSampler, input.Tex);
  if (color.a < 0.5)
  {
      discard;
  }
  
  return color;
}