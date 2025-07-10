#include "ShaderConstants.hlsl"

SamplerState textureSampler : register(s0);
Texture2D t_depthMap : register(t0);
Texture2D t_normalMap : register(t1);
Texture2D t_colorMap : register(t2);
Texture2D t_propMap : register(t3);
Texture2D t_aoMap : register(t4);
Texture2D t_shadowMap : register(t5);
Texture2D t_skyMap : register(t6);
RWTexture2D<float4> t_outputMap : register(u0);

#ifndef PCF_KERNEL_SIZE
#define PCF_KERNEL_SIZE 5
#endif
#ifndef DELTA
#define DELTA 0.00000001
#endif
#ifndef PI
#define PI 3.14159265359
#endif

cbuffer Light : register(b2)
{
  float4 LightPos[12];
}

cbuffer LightCam : register(b3)
{
  float4x4 lightView;
  float4x4 lightProj;
}

struct PS_INPUT
{
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

float
pcFiltering(float2 uv,
            float depth,
            float texelSize,
            float shadowBias)
{
  float shadow = 0.0f;
  //int sampleCount = 0;

  for (int y = -PCF_KERNEL_SIZE; y <= PCF_KERNEL_SIZE; ++y)
  {
    for (int x = -PCF_KERNEL_SIZE; x <= PCF_KERNEL_SIZE; ++x)
    {
    
      float2 offset = float2(x, y) * texelSize;
      //float sampledDepth = t_shadowMap.Sample(textureSampler, uv + offset).r;
      float sampledDepth = t_shadowMap.Load(uint3(uv + offset, 0)).r;

      sampledDepth = sampledDepth * 0.5f + 0.5f;
      float shadowIntensity = 0.8f; // Change to a variable in constant buffer
      shadow += depth > sampledDepth + shadowBias ? shadowIntensity : 1.0f;
      //if (depth - shadowBias > sampledDepth)
      //{
      //  shadow += 1.0f;
      //}
      //++sampleCount;
    }
  }
    
  shadow /= PCF_KERNEL_SIZE * PCF_KERNEL_SIZE;
  return saturate(shadow);
  //return 1.0f - (shadow / sampleCount);
}

float3 fresnelSchlick(float3 F0, float cosTheta)
{
  return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

// Prince function
float geomSmith(float nDotV, float nDotL, float roughness)
{
  float k = pow((roughness + 1.0f), 2.0f) * 0.125f;
  float ik = 1.0f - k;
  nDotV = max(nDotV, DELTA);
  nDotL = max(nDotL, DELTA);
  float ggx1 = nDotV / (nDotV * ik + k);
  float ggx2 = nDotL / (nDotL * ik + k);
  return ggx1 * ggx2;
}

// Ndf Beckmann
float D_Beckmann(float nDotH, float alpha)
{
  float alpha2 = alpha * alpha;
  float cos2 = nDotH * nDotH;
  float tan2 = (1.0f - cos2) / (cos2 + 1e-5f);
  return exp(-tan2 / alpha2) / (PI * alpha2 * cos2 * cos2 + 1e-5f);
}

// Ndf Bling-Phong
float D_BlinnPhong(float NdotH, float roughness)
{
    float alpha = (2.0 / (roughness * roughness)) - 2.0;
    return ((alpha + 2.0) / (2.0 * 3.14159265)) * pow(saturate(NdotH), alpha);
}

float geometrySchlick(float nDotV, float roughness)
{
  float r = roughness + 1.0f;
  float k = (r * r) / 8.0f;
  float ik = 1.0f - k;
  return nDotV / (nDotV * ik + k);
}

float geometrySmith(float nDotV, float nDotL, float roughness)
{
  float ggx1 = geometrySchlick(nDotV, roughness);
  float ggx2 = geometrySchlick(nDotL, roughness);
  return ggx1 * ggx2;
}

float3 cookTorrenceSpecular(float3 normal,
                            float3 viewDirection,
                            float3 lightDirection,
                            float roughness,
                            float3 F0)
{
  float3 H = normalize(viewDirection + lightDirection);
  float nDotL = saturate(dot(normal, lightDirection));
  float nDotV = saturate(dot(normal, viewDirection));
  float nDotH = saturate(dot(normal, H));
  float vDotH = saturate(dot(viewDirection, H));
    
  float alpha = roughness * roughness;
  //float D = D_Beckmann(nDotH, alpha);
  float D = D_BlinnPhong(nDotH, roughness);
  //float G = geometrySmith(nDotV, nDotL, roughness);
  float G = geomSmith(nDotV, nDotL, roughness);
  float F = fresnelSchlick(F0, vDotH);
    
  float denominator = 4.0f * nDotV * nDotL + 1e-5f;
  return (D * G * F) / denominator;
}

[numthreads(32, 32, 1)]
void CSMain(uint3 dtID : SV_DispatchThreadID)
{
  if (dtID.x >= screenSize.x || dtID.y >= screenSize.y) {
    return;
  }
  
  float2 screenUV = dtID.xy / screenSize;
  
  float4 depth = t_depthMap.Load(int3(dtID.xy, 0));
  float4 normalMap = t_normalMap.Load(int3(dtID.xy, 0));
  float4 color = t_colorMap.Load(int3(dtID.xy, 0));
  float4 propMap = t_propMap.Load(int3(dtID.xy, 0));
  float4 ao = t_aoMap.Load(int3(dtID.xy, 0));
  float4 shadows = t_shadowMap.Load(int3(dtID.xy, 0));
  
  float3 albedo = color.rgb;
  float metalness = propMap.r;
  float3 normal = normalMap.xyz;
  float roughness = propMap.b;
  
  if(color.a < 0.5f)
  {
    t_outputMap[dtID.xy] = float4(1.0f, 1.0f, 1.0f, 0.0f);
    return;
  }
  //float normalLen = length(normalMap.xyz);
  //if(normalLen < 0.001f) {
  //  t_outputMap[dtID.xy] = float4(t_skyMap.Load(uint3(dtID.xy, 0)).rgb, 1.0f);
  //}
  if (normalMap.w == 0)
  {
    t_outputMap[dtID.xy] = float4(1.0f, 1.0f, 1.0f, 0.0f);
    return;
  }
  
  normal = normal * 2.0f - 1.0f;
  float4 posWorld = depth;
  
  // Light calculations
  float3 lightDir = normalize(LightPos[0].xyz - posWorld.xyz);
  float3 viewDirection = normalize(viewPos.xyz - posWorld.xyz);
  float NdL = saturate(dot(normal, lightDir));
  
  float3 F0 = lerp(0.04, albedo, metalness);
  
  float3 ambientLight = 0.15f * albedo;
  
  float3 specular = cookTorrenceSpecular(normal, viewDirection, lightDir, roughness, F0);
  float3 finalColor = ((((albedo + specular) * lightIntensity * NdL) + ambientLight) * ao.r);
  
  // Shadow calculation
  float4 lightWorldPos = mul(posWorld, mul(lightView, lightProj));
  lightWorldPos.xyz /= lightWorldPos.w;
  lightWorldPos.xyz = lightWorldPos.xyz * 0.5f + 0.5f;
  
  float2 shadowCoord = lightWorldPos.xy;
  shadowCoord.y = 1.0f - shadowCoord.y;
  float shadowFactor = 1.0f;
  if(shadowCoord.x < 0.0f || shadowCoord.x > 1.0f ||
     shadowCoord.y < 0.0f || shadowCoord.x > 1.0f) {
    t_outputMap[dtID.xy] = float4(ambientLight, 1.0f);
    return;
  }
  
  float shadowBias = max(0.001f * (1.0f - NdL), 0.001f);
  
  float texelSize = 1.0f / shadowMapSize;
  
  shadowFactor = pcFiltering(shadowCoord, lightWorldPos.z, texelSize, shadowBias);
  
  finalColor *= shadowFactor;
  t_outputMap[dtID.xy] = float4(finalColor, 1.0f);
}