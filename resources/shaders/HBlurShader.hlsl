SamplerState textureSampler : register(s0);
Texture2D t_AOMap : register(t0);

cbuffer Viewport : register(b0)
{
  float2 Dimensions;
  float unused;
  float unused1;
}

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 Texcoord : TEXCOORD0;
};

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
  float2 uv = input.Texcoord / Dimensions;
  float2 texelSize = float2(1.0f / Dimensions.x, 1.0f / Dimensions.y);
    
  float2 offsets[5] =
  {
    float2(-2.0f, 0.0f) * texelSize,
    float2(1.0f, 0.0f) * texelSize,
    float2(0.0f, 0.0f),
    float2(1.0f, 0.0f) * texelSize,
    float2(2.0f, 0.0f) * texelSize
  };

  float kernel[5] = { 0.1f, 0.2f, 0.4f, 0.2f, 0.1f };
    
  float3 sum = float3(0.0f, 0.0f, 0.0f);
    
  for (int i = 0; i < 5; ++i)
  {
    float3 color = t_AOMap.Sample(textureSampler, uv + offsets[i]).rgb;
    sum += color * kernel[i];
  }
    
  return float4(sum, 1.0f);
}