SamplerState textureSampler : register(s0);
Texture2D t_inputMap : register(t0);
Texture3D<float4> lutTex : register(t1);
RWTexture2D<float4> t_outputMap : register(u0);

cbuffer Viewport : register(b0)
{
  float2 ScreenSize;
  float unused;
  float unused1;
}

cbuffer ToneIndex : register(b1)
{
  float index;
  float lutSize;
  float whitePoint;
  float exposure;
}

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

[numthreads(32, 32, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  if (dtID.x >= ScreenSize.x || dtID.y >= ScreenSize.y) {
    return;
  }

  float4 color = t_inputMap.Load(uint3(dtID.xy, 0));
  color *= exposure;
  
  if(index == 0.0f)
  {
    color.rgb = reinhard(color.rgb);
  }
  else if(index == 1.0f)
  {
    color.rgb = aces(color.rgb);
  }
  else if(index == 2.0f)
  {
    color.rgb = uncharted2(color.rgb);
  }
  else if(index == 3.0f)
  {
    color.rgb = agx(color.rgb);
  }
  else if(index == 4.0f)
  {
    color.rgb = lutToneMap(color.rgb);
  }
    
  color = pow(color, 1.0 / 2.2);
    
  t_outputMap[dtID.xy] = float4(color.rgb, 1.0f);
}