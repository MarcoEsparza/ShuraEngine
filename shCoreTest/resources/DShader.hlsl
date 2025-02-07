sampler2D DepthMap : register(s0);
sampler2D NormalMap : register(s1);
sampler2D ColorMap : register(s2);

cbuffer VP : register(b0)
{
  float4x4 View;
  float4x4 Proj;
}

cbuffer InvVP : register(b1)
{
  float4x4 InvViewProj;
}

cbuffer ViewDir : register(b2)
{
  float4 ViewPos;
}

cbuffer Light : register(b3)
{
  float3 LightPosition;
  float Intensity;
  float4 LightColor;
  //float size[10];
}

cbuffer Viewport : register(b4)
{
  float2 Dimensions;
  float2 unused;
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
  
  //output.Position = mul(float4(input.Position.xyz, 1.0f), mul(View, Proj));
  output.Position = input.Position;
    
  return output;
}

float4 mainPS(PS_INPUT input) : SV_Target
{
  float2 screenUV = input.Position.xy;
    
  float4 depth = tex2D(DepthMap, screenUV);
    
  return depth;
}