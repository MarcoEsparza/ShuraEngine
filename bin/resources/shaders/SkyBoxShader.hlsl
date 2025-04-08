SamplerState textureSampler : register(s0);
Texture2D t_skybox : register(t0);

#ifndef PI
#define PI 3.14159265359
#endif
#ifndef RECIPROCAL_PI
#define RECIPROCAL_PI 1.0f / 3.14159265359
#endif
#ifndef RECIPROCAL_2PI
#define RECIPROCAL_2PI 1.0f / (2 * 3.14159265359)
#endif

cbuffer VP : register(b0)
{
  float4x4 matView;
  float4x4 matProj;
}

cbuffer InvVP : register(b1)
{
  float4x4 InvViewProj;
  float4x4 InvView;
}

cbuffer Viewport : register(b2)
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
  float4 Position : SV_Position;
  float3 Texcoord : TEXCOORD0;
};

float2 getSkyBoxUV(float3 dir)
{
  float u = -atan2(dir.z, dir.x) * RECIPROCAL_2PI + 0.5f;
  float v = acos(dir.y) * RECIPROCAL_PI;
  return float2(u, v);
}

PS_INPUT main(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT) 0;
    
  matrix newViewMatrix = matView;
  newViewMatrix[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
  float4 pos = float4(input.Position, 1.0f);
  pos = mul(pos, newViewMatrix);
  pos = float4(pos.xyz, 1.0f);
    
  pos = mul(pos, matProj);
  pos.z = pos.w;
  output.Position = pos;
  output.Texcoord = input.Position.xyz;
    
  return output;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
  //float2 ndc = input.Position.xy / Dimensions * 2.0f - 1.0f;
  //ndc.y = -ndc.y;
    
  //float4 viewPos = mul(float4(ndc, 1.0f, 1.0f), InvView);
  //viewPos.y /= viewPos.w;
    
  //float3 viewDir = normalize(viewPos.xyz);
  //float uv = getSkyBoxUV(normalize(viewDir));

  float uv = getSkyBoxUV(normalize(input.Texcoord));
    
  float3 color = t_skybox.Sample(textureSampler, uv).xyz;
  return float4(color, 1.0f);
}