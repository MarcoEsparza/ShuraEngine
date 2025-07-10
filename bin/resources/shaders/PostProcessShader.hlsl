#include "ShaderConstants.hlsl"

SamplerState textureSampler : register(s0);
SamplerState samplerLinearClamp : register(s1);
Texture2D t_inputMap : register(t0);
Texture2D t_texture1 : register(t1);
Texture2D t_texture2 : register(t2);
Texture3D<float4> lutTex : register(t3);
RWTexture2D<float4> t_outputMap : register(u0);

#ifndef DELTA
#define DELTA 0.00000001
#endif

#define BLURH_THREADS_X 4
#define BLURH_THREADS_Y 64
#define BLURV_THREADS_X 64
#define BLURV_THREADS_Y 4
#define BLUR_WIDTH 9
#define HALF_BLUR_WIDTH 4

/**
*  @brief Reinhard filmic tone mapping
*
*  @param float3 hdrColor
*
*  @return float3 Mapped color
*/
float3
reinhard(float3 hdrColor)
{
  return (hdrColor * (1.0f + hdrColor / (whitePoint * whitePoint))) / (hdrColor + 1.0f);
}

float3
RRTAndODTFit(float3 v)
{
  float3 a = v * (v + 0.0245786f) - 0.000090537f;
  float3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
  return a / b;
}

float3
aces(float3 color)
{
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
    
  color = (color * (a * color + b)) / (color * (c * color + d) + e);
    
  color = saturate(color);
  return color;
    
  //const float3x3 ACESInputMat =
  //{
  //  { 0.59719f, 0.35458f, 0.04823f },
  //  { 0.07600f, 0.90834f, 0.01566f },
  //  { 0.02840f, 0.13383f, 0.83777f }
  //};

  //const float3x3 ACESOutputMat =
  //{
  //  { 1.60475f, -0.53108f, -0.07367f },
  //  { -0.10208f, 1.10813f, -0.00605f },
  //  { -0.00327f, -0.07276f, 1.07602f }
  //};

  //color = mul(ACESInputMat, color);
  //color = RRTAndODTFit(color);
  //color = mul(ACESOutputMat, color);
  //return saturate(color);
}

float3
uncharted2(float3 x)
{
  float A = 0.15f;
  float B = 0.50f;
  float C = 0.10f;
  float D = 0.20f;
  float E = 0.02f;
  float F = 0.30f;
  float W = 11.2f;

  float3 mapped = ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
  //float whiteScale = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
  //return mapped / whiteScale;
    
  mapped *= whitePoint;
  return mapped;
}

float3
agx(float3 X)
{
  //float3 a = log2(color + 1e-6);
  //float3 b = saturate((a + 5.0) / 10.0);
  //return pow(b, 1.0 / 2.2);
    
  X = max(X, 0.0);
  
  const float a = 2.521;
  const float b = 0.03;
  const float c = 2.437;
  const float d = 0.4;
  const float e = 0.03;
  
  X = (X * (a * X + b)) / (X * (c * X + d) + e);
    
  return saturate(X);
}

float3
lutToneMap(float3 color)
{
  color = saturate(color);
  float3 coord = color * (lutSize - 1.0) / lutSize;
  return lutTex.SampleLevel(textureSampler, coord, 0).rgb;
}

float3
getSelectedToneMap(float3 color, float toneMapIndex)
{
  if (toneMapIndex == 0.0f) {
    return reinhard(color);
  }
  else if (toneMapIndex == 1.0f) {
    return aces(color);
  }
  else if (toneMapIndex == 2.0f) {
    return uncharted2(color);
  }
  else if (toneMapIndex == 3.0f) {
    return agx(color);
  }
  else if (toneMapIndex == 4.0f) {
    return lutToneMap(color);
  }
  
  return color; // Default case, no tone mapping
}

[numthreads(32, 32, 1)]
void
PostProcessCS(uint3 dtID : SV_DispatchThreadID)
{
  if (dtID.x >= uint(screenSize.x) || dtID.y >= uint(screenSize.y)) {
    return;
  }

  float4 color = t_inputMap.Load(uint3(dtID.xy, 0));
    
  color = saturate(color);
    
  color.r = lerp(minR, maxR, color.r);
  color.g = lerp(minG, maxG, color.g);
  color.b = lerp(minB, maxB, color.b);
    
  color = pow(color, 1.0f / 2.2f);
  
  t_outputMap[dtID.xy] = color;
}

[numthreads(32, 32, 1)]
void
ToneMapCS(uint3 dtID : SV_DispatchThreadID)
{
  uint2 dimensions;
  t_outputMap.GetDimensions(dimensions.x, dimensions.y);
  if (dtID.x >= dimensions.x || dtID.y >= dimensions.y)
  {
    return;
  }
  float2 uv = (float2(dtID.x, dtID.y) + 0.5f) / float2(dimensions.x, dimensions.y);

  float3 color = t_inputMap.Load(uint3(dtID.xy, 0)).rgb;
  
  uint2 bloomDimensions;
  t_texture1.GetDimensions(bloomDimensions.x, bloomDimensions.y);
  uint2 bloomUV = uint2(uv.x * bloomDimensions.x, uv.y * bloomDimensions.y);
  float3 bloom = t_texture1.Load(uint3(bloomUV, 0)).rgb;
  //color *= exposure;
    
  float3 texDimensions;
  t_texture2.GetDimensions(0, texDimensions.x, texDimensions.y, texDimensions.z);
  float avgLogLum = t_texture2.SampleLevel(samplerLinearClamp,
                                           float2(0.5f, 0.5f),
                                           texDimensions.z).r;
  float avgLum = exp(avgLogLum);
    
  float3 exposed = color * middleGrey / (avgLum + DELTA);
  float3 mapped = getSelectedToneMap(exposed, toneMapIndex);
    
  //mapped = pow(mapped, 1.0f / 2.2f);
  //float bloomMultiplier = 1.0f;
  mapped += bloom * bloomMultiplier;
    
  t_outputMap[dtID.xy] = float4(saturate(mapped), 1.0f);
}

[numthreads(1, BLURH_THREADS_Y, 1)]
void
HBlur_CS(uint3 gID : SV_GroupID,
         uint3 dtID : SV_DispatchThreadID,
         uint3 gtID : SV_GroupThreadID,
         uint GI : SV_GroupIndex)
{
  //int2 outputDimensions;
  //t_outputMap.GetDimensions(outputDimensions.x, outputDimensions.y);
  //if (dtID.x >= outputDimensions.x || dtID.y >= outputDimensions.y) {
  //  return;
  //}
    
  static const float g_blurWeights[] = {
    0.004815026f,
    0.028716039f,
    0.102818575f,
    0.221024189f,
    0.28525234f,
    0.221024189f,
    0.102818575f,
    0.028716039f,
    0.004815026f
  };
  int3 base = int3(dtID.x * BLURH_THREADS_X, dtID.y, mipLevel0);
  float4 input[BLUR_WIDTH + BLURH_THREADS_X];
  
  [unroll]for(int i = 0; i < BLUR_WIDTH + BLURH_THREADS_X; i++) {
    input[i] = t_inputMap.Load(base, int2(i - HALF_BLUR_WIDTH, 0));
  }
  
  [unroll]for(int x = 0; x < BLURH_THREADS_X; x++) {
    float4 output = 0.0f;
    [unroll]for(int i = 0; i < BLUR_WIDTH; i++) {
      output += input[(x + i)] * g_blurWeights[i];
    }
    t_outputMap[base.xy + int2(x, 0)] = output;
  }
}

[numthreads(BLURV_THREADS_X, 1, 1)]
void
VBlur_CS(uint3 gID : SV_GroupID,
         uint3 dtID : SV_DispatchThreadID,
         uint3 gtID : SV_GroupThreadID,
         uint GI : SV_GroupIndex)
{
  static const float g_blurWeights[] = {
    0.004815026f,
    0.028716039f,
    0.102818575f,
    0.221024189f,
    0.28525234f,
    0.221024189f,
    0.102818575f,
    0.028716039f,
    0.004815026f
  };
  int3 base = int3(dtID.x, dtID.y * BLURV_THREADS_Y, mipLevel0);
  float4 input[BLUR_WIDTH + BLURV_THREADS_Y];
  
  [unroll]for (int i = 0; i < BLUR_WIDTH + BLURV_THREADS_Y; i++) {
    input[i] = t_inputMap.Load(base, int2(0, i - HALF_BLUR_WIDTH));
  }
  
  [unroll]for(int y = 0; y < BLURV_THREADS_Y; y++) {
    float4 output = 0.0f;
    [unroll]for(int i = 0; i < BLUR_WIDTH; i++) {
      output += input[(y + i)] * g_blurWeights[i];
    }
    t_outputMap[base.xy + int2(0, y)] = output;
  }
}

[numthreads(32, 32, 1)]
void
HBlurCS(uint3 dtID : SV_DispatchThreadID)
{
  uint2 dimensions;
  t_outputMap.GetDimensions(dimensions.x, dimensions.y);
  if (dtID.x >= dimensions.x || dtID.y >= dimensions.y) {
    return;
  }

  float2 coord = dtID.xy / dimensions;
  float2 texelSize = float2(1.0f / dimensions.x, 1.0f / dimensions.y);

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
    float3 color = t_inputMap.Load(int3(dtID.xy, 0)).rgb;
    sum += color * kernel[i];
  }

  t_outputMap[dtID.xy] = float4(sum, 1.0f);
}

[numthreads(32, 32, 1)]
void
VBlurCS(uint3 dtID : SV_DispatchThreadID)
{
  uint2 dimensions;
  t_outputMap.GetDimensions(dimensions.x, dimensions.y);
  if (dtID.x >= dimensions.x || dtID.y >= dimensions.y)
  {
    return;
  }

  float2 coord = dtID.xy / dimensions;
  float2 texelSize = float2(1.0f / dimensions.x, 1.0f / dimensions.y);

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