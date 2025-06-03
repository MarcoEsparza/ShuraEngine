Texture2D t_inputMap : register(t0);
RWTexture2D<float4> t_outputMap : register(u0);

cbuffer Viewport : register(b0)
{
  float2 Dimensions;
  float unused;
  float unused1;
}

[numthreads(32, 32, 1)]
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
    float2(0.0f, -2.0f) * texelSize,
    float2(0.0f, 1.0f) * texelSize,
    float2(0.0f, 0.0f),
    float2(0.0f, 1.0f) * texelSize,
    float2(0.0f, 2.0f) * texelSize
  };

  float kernel[5] = { 0.1f, 0.2f, 0.4f, 0.2f, 0.1f };

  float3 sum = float3(0.0f, 0.0f, 0.0f);

  for (int i = 0; i < 5; ++i)
  {
    float3 color = t_inputMap.Load(int3(dtID.xy, 0)).rgb;
    sum += color * kernel[i];
  }

  t_outputMap[dtID.xy] = float4(sum, 1.0f);
}