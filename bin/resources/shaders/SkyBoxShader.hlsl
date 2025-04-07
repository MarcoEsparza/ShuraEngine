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
  pos = mul(pos, matProj);
  pos.z = pos.w;
  output.Position = pos;
  output.Texcoord = input.Position.xyz;
    
  return output;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
  float uv = getSkyBoxUV(normalize(input.Texcoord));
    
  float3 color = t_skybox.Sample(textureSampler, uv).xyz;
  return float4(color, 1.0f);
}