SamplerState textureSampler : register(s0);
Texture2D t_baseColor : register(t0);
Texture2D t_normal : register(t1);
Texture2D t_metallic : register(t2);
Texture2D t_roughness : register(t3);
Texture2D t_ambientO : register(t4);

Texture2D t_posMap : register(t5);
Texture2D t_depthMap : register(t6);
Texture2D t_normalMap : register(t7);
Texture2D t_colorMap : register(t8);
Texture2D t_aoMap : register(t9);
Texture2D t_shadowMap : register(t10);

#ifndef PCF_KERNEL_SIZE
#define PCF_KERNEL_SIZE 5
#endif

cbuffer MainBuffer : register(b0)
{
  float4x4 matView;
  float4x4 matViewTranspose;
  float4x4 matViewInverse;
  float4x4 matViewInverseTranspose;

  float4x4 matProjection;
  float4x4 matProjectionTranspose;
  float4x4 matProjectionInverse;
  float4x4 matProjectionInverseTranspose;
    
  float2 viewPort;
  float nearClip;
  float farClip;
    
  float4 viewPos;
  float4 viewTarget;
}

cbuffer AOSettings : register(b8)
{
  float2 ScreenSize;
  float RandomSize;
  float SampleRad;
  float Scale;
  float Bias;
  float Intensity;
  float unused;
}

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
