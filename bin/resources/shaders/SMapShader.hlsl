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
};

struct PS_INPUT
{
  float4 Position : SV_POSITION;
};

PS_INPUT main(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT) 0;
  
  float4x4 wvp = mul(ModelTransform, mul(View, Proj));
  output.Position = mul(float4(input.Position.xyz, 1.0f), wvp);
  
  return output;
}

float4 mainPS(PS_INPUT input) : SV_Target
{
  return 1.0f;
}