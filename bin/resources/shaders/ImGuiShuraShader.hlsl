SamplerState textureSampler : register(s0);
Texture2D t_color : register(t0);

cbuffer VertexBuffer : register(b0)
{
  float4x4 Proj;
}

struct VS_INPUT
{
  float2 pos : POSITION;
  float2 uv : TEXCOORD0;
  float4 col : COLOR0;
};

struct PS_INPUT
{
  float4 pos : SV_POSITION;
  float4 col : COLOR0;
  float2 uv : TEXCOORD0;
};

PS_INPUT main(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT)0;

  output.pos = mul(float4(input.pos, 1.0f, 1.0f), Proj);
  output.col = input.col;
  output.uv = input.uv;
    
  return output;
}

float4 mainPS(PS_INPUT input) : SV_Target
{
  return input.col * t_color.Sample(textureSampler, input.uv);
}