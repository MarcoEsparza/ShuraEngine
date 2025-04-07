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

PS_INPUT main(uint vertexID : SV_VertexID)
{
  PS_INPUT output = (PS_INPUT) 0;
  
  float2 position[3] =
  {
    float2(-1.0f, -1.0f),
    float2(3.0f, -1.0f),
    float2(-1.0f, 3.0f)
  };
  
  output.Position = float4(position[vertexID], 0.0f, 1.0f);
  output.Texcoord = (position[vertexID] + float2(1.0f, 1.0f)) * 0.5f;
  output.Texcoord.y = 1.0f - output.Texcoord.y;
    
  return output;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
  float2 screenUV = input.Position.xy / Dimensions;
    
  float4 normal = getNormal(screenUV);
  if(normal.w == 0.0f)
  {
    clip(-1);
  }
    
  float4 skyBoxColor = t_skybox.Sample(textureSampler, screenUV);
    
  return skyBoxColor;
}