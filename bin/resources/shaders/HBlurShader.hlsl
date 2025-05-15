SamplerState textureSampler : register(s0);
Texture2D t_AOMap : register(t0);
RWTexture2D<float4> t_outputMap : register(u0);

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

//float4 mainPS(PS_INPUT input) : SV_TARGET
//{
//  float2 uv = input.Position.xy / Dimensions;
//  float2 texelSize = float2(1.0f / Dimensions.x, 1.0f / Dimensions.y);
    
//  float2 offsets[5] =
//  {
//    float2(-2.0f, 0.0f) * texelSize,
//    float2(1.0f, 0.0f) * texelSize,
//    float2(0.0f, 0.0f),
//    float2(1.0f, 0.0f) * texelSize,
//    float2(2.0f, 0.0f) * texelSize
//  };

//  float kernel[5] = { 0.1f, 0.2f, 0.4f, 0.2f, 0.1f };
    
//  float3 sum = float3(0.0f, 0.0f, 0.0f);
    
//  for (int i = 0; i < 5; ++i)
//  {
//    float3 color = t_AOMap.Sample(textureSampler, uv + offsets[i]).rgb;
//    sum += color * kernel[i];
//  }
    
//  return float4(sum, 1.0f);
//}

[numthreads(16, 16, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  if (dtID.x >= Dimensions.x || dtID.y >= Dimensions.y) {
    return;
  }

  float2 coord = dtID.xy / Dimensions;
  float2 texelSize = float2(1.0f / Dimensions.x, 1.0f / Dimensions.y);

  float2 offsets[5] =
  {
    float2(-2.0f, 0.0f)* texelSize,
    float2(1.0f, 0.0f)* texelSize,
    float2(0.0f, 0.0f),
    float2(1.0f, 0.0f)* texelSize,
    float2(2.0f, 0.0f)* texelSize
  };

  float kernel[5] = { 0.1f, 0.2f, 0.4f, 0.2f, 0.1f };

  float3 sum = float3(0.0f, 0.0f, 0.0f);

  for (int i = 0; i < 5; ++i)
  {
    float3 color = t_AOMap.Load(int3(dtID.xy, 0)).rgb;
    sum += color * kernel[i];
  }

  t_outputMap[dtID.xy] = float4(sum, 1.0f);
}