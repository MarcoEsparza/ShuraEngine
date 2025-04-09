//#include "resources/shaders/shPostProccessFunctions.hlsl"
//#include "resources/shaders/shMainConstantBuffer.hlsl"

SamplerState textureSampler : register(s0);
Texture2D t_posMap : register(t0);
Texture2D t_normalMap : register(t1);

cbuffer AOSettings : register(b0)
{
  float2 ScreenSize;
  float SampleRad;
  float Scale;
  float Bias;
  float Intensity;
  float2 unused;
}

struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 Texcoord : TEXCOORD0;
};

float3 getPosition(in float2 uv)
{
  return t_posMap.Sample(textureSampler, uv).rgb;
}

float4 getNormal(in float2 uv)
{
  float4 normal = t_normalMap.Sample(textureSampler, uv);
  normal.xyz = normal.xyz * 2.0f - 1.0f;
  return normal;
}

float2 getRandom(in float2 uv)
{
  float noiseX = (frac(sin(dot(uv, float2(15.8989f, 76.132f) * 1.0f)) * 46336.23745f));
  float noiseY = (frac(sin(dot(uv, float2(11.9899f, 62.223f) * 2.0f)) * 34748.34744f));
  float noiseZ = (frac(sin(dot(uv, float2(13.3238f, 63.122f) * 3.0f)) * 59998.47362f));
    
  return normalize(float3(noiseX, noiseY, noiseZ));
}

float computeAO(in float2 tcood, in float2 uv, in float3 p, in float3 cnorm)
{
  float3 diff = getPosition(tcood + uv).xyz - p;
  const float d = length(diff) * Scale;
  const float v = normalize(diff);
  return max(0.0f, dot(cnorm, v) - Bias) * (1.0f / (1.0f + d)) * Intensity;
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
  float2 screenUV = input.Position.xy / ScreenSize;
    
  float4 normal = getNormal(screenUV);
  if(normal.w == 0.0f)
  {
    clip(-1);
  }
    
  float3 pos = getPosition(screenUV);
  float3 n = normal.xyz;
  float2 rand = getRandom(screenUV);
    
  float ao = 0.0f;
  float rad = SampleRad / -pos.x;
    
  float2 vec[4] =
  {
   float2(1.0f, 0.0f),
   float2(-1.0f, 0.0f),
   float2(0.0f, 1.0f),
   float2(0.0f, -1.0f)
  };

  int iter = 4;
  for(int j = 0; j < iter; ++j)
  {
    float2 coord1 = reflect(vec[j], rand) * rad;
    float2 coord2 = float2(coord1.x * 0.707 - coord1.y * 0.707,
                           coord1.x * 0.707 - coord1.y * 0.707);
       
    ao += computeAO(screenUV, coord1 * 0.25f, pos.xyz, n);
    ao += computeAO(screenUV, coord2 * 0.5f,  pos.xyz, n);
    ao += computeAO(screenUV, coord2 * 0.75f, pos.xyz, n);
    ao += computeAO(screenUV, coord2,         pos.xyz, n);
  }
    
  ao /= (iter * 4);
    
  return 1.0f - ao;
}