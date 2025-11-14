#include "ShaderConstants.hlsl"

Texture2D t_depthMap : register(t0);
Texture2D t_normalMap : register(t1);
Texture2D t_colorMap : register(t2);
Texture2D t_propMap : register(t3);
//Texture2D t_emmisiveMap : register(t4);
Texture2D t_ssaoMap : register(t4);
Texture2D t_shadowMap : register(t5);
Texture2D t_depthStencil : register(t6);
Texture2D t_brdfLUT : register(t7);
Texture2D t_diffIrr : register(t8);
Texture2D t_skyReflect : register(t9);
RWTexture2D<float4> t_outputMap : register(u0);

#define PCF_KERNEL_SIZE 5
#define DELTA 0.00000001
#define SAMPLE_DELTA 0.2f
#define MAX_REFLECTION_LOD 5.0f

cbuffer LightData : register(b2)
{
  float4 LightPos;
  float3 lightTarget;
  float lightIntensity;
  float4 lightColor;
  float4x4 lightView;
  float4x4 lightProj;
}

cbuffer PrefilterConstants : register(b3)
{
  uint width;
  uint height;
  uint samples;
  float prefRoughness;
  float mipmapLevels;
  float3 pcPadding; // Padding to 16 bytes
};

//float
//pcFiltering(float2 uv,
//            float depth,
//            float texelSize,
//            float shadowBias)
//{
//  float shadow = 0.0f;
//  //int sampleCount = 0;

//  for (int y = -PCF_KERNEL_SIZE; y <= PCF_KERNEL_SIZE; ++y)
//  {
//    for (int x = -PCF_KERNEL_SIZE; x <= PCF_KERNEL_SIZE; ++x)
//    {
  
//      float2 offset = float2(x, y) * texelSize;
//      float sampledDepth = t_shadowMap.Load(uint3(uv + offset, 0)).r;

//      sampledDepth = sampledDepth * 0.5f + 0.5f;
//      float shadowIntensity = 0.8f; // Change to a variable in constant buffer
//      shadow += depth > sampledDepth + shadowBias ? shadowIntensity : 1.0f;
//    }
//  }
  
//  shadow /= PCF_KERNEL_SIZE * PCF_KERNEL_SIZE;
//  return saturate(shadow);
//  //return 1.0f - (shadow / sampleCount);
//}

float
pcFiltering(float2 uv,
            float currentDepth,
            float texelSize,
            float bias)
{
  float shadow = 0.0f;
  [unroll]
  for (int x = -1; x <= 1; x++)
  {
    [unroll]
    for (int y = -1; y <= 1; y++)
    {
      float2 offset = float2(x, y) * texelSize;
      float depth = t_shadowMap.SampleLevel(samplerLinearClamp, uv + offset, 0).r;
      shadow += currentDepth - bias > depth ? 0.0f : 1.0f;
    }
  }
  return shadow /= 9.0f;
}

float
shadowCalculation(float4 lightSpacePos, float NdL)
{
  // Perform perspective divide
  float3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
  // Transform to [0,1] range
  projCoords = projCoords * 0.5f + 0.5f;
  // Get closest depth value from light's perspective (using [0,1] range lightSpacePos as coords)
  float closestDepth = t_shadowMap.SampleLevel(samplerLinearClamp, projCoords.xy, 0).r;
  // Get depth of current fragment from light's perspective
  float currentDepth = projCoords.z;
  // Check whether current frag pos is in shadow
  float shadowBias = max(0.05f * (1.0f - NdL), 0.005f);
  float shadow = currentDepth - shadowBias > closestDepth ? 1.0f : 0.0f;
  float texelSize = 1.0f / shadowMapSize;
  
  // PCF
  shadow = pcFiltering(projCoords.xy, currentDepth, texelSize, shadowBias);
  
  // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
  if (projCoords.z > 1.0f) {
    shadow = 0.0f;
  }
  
  return shadow;
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
  float D = D_Beckmann(nDotH, alpha);
  //float D = D_BlinnPhong(nDotH, roughness);
  //float G = geometrySmith(nDotV, nDotL, roughness);
  float G = geomSmith(nDotV, nDotL, roughness);
  float3 F = fresnelSchlick(F0, vDotH);

  float denominator = 4.0f * nDotV * nDotL + 1e-5f;
  return (D * G * F) / denominator;
}

float3
Lambert(float3 fresnel, float3 albedo, float metallic)
{
  // Lambertian diffuse reflectance with energy conservation
  float3 diffuse = (1.0f - fresnel) * albedo * (1.0f - metallic);
  return diffuse;
}

float4 getSpecularSample(float3 reflection, float lod)
{
  float u_EnvIntensity = 1.0f; // Environment intensity, can be adjusted
  
  float2 uv = getSkyBoxUV(normalize(reflection));
  // Sample the texture at the specified LOD level
  float4 texSample = t_skyReflect.SampleLevel(samplerAnisotropicClamp, uv, lod);

  texSample.rgb *= u_EnvIntensity; // Apply environment intensity
  return float4(texSample.rgb, 1.0f);
}

float3 getIBLRadianceGGX(float3 n, float3 v, float roughness)
{
  float NdotV = clampedDot(n, v);

  float3 texDimensions;
  t_skyReflect.GetDimensions(0, texDimensions.x, texDimensions.y, texDimensions.z);
  float lod = min(roughness * texDimensions.x, texDimensions.z);

  float3 reflection = normalize(reflect(-v, n));
  float4 specularSample = getSpecularSample(reflection, lod);

  return specularSample.rgb;
}

float3
getIBLGGXFresnel(float3 n, float3 v, float roughness, float3 F0, float specularWeight)
{
  // Roughness dependent Fresnel
  float nDotV = clampedDot(n, v);
  float2 brdfSamplePoint = clamp(float2(nDotV, 1.0f - roughness), 0.0f, 1.0f);
  float2 f_ab = t_brdfLUT.SampleLevel(samplerPointClamp, brdfSamplePoint, 0);
  float3 Fr = max(1.0f - roughness, F0) - F0;
  float3 kS = F0 + Fr * pow(1.0f - nDotV, 5.0f);
  float3 FssEss = specularWeight * (kS * f_ab.x + f_ab.y);
  
  // Multiple scattering
  float Ems = (1.0f - (f_ab.x + f_ab.y));
  float3 F_avg = specularWeight * (F0 + (1.0F - F0) / 21.0F);
  float3 FmsEms = Ems * FssEss * F_avg / (1.0f - F_avg * Ems);
  
  return FssEss + FmsEms;
}

float3
getDiffuseLight(float3 n)
{
  float envIntensity = 1.0f;
  float2 dir = getSkyBoxUV(n);
  float3 texSample = t_diffIrr.SampleLevel(samplerAnisotropicClamp, dir, 0).rgb;
  texSample *= envIntensity; // Apply environment intensity
  return texSample;
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
  //float4 emmisiveMap = t_emmisiveMap.Load(int3(dtID.xy, 0));
  float4 ssaoMap = t_ssaoMap.Load(int3(dtID.xy, 0));
  float4 shadows = t_shadowMap.Load(int3(dtID.xy, 0));
  
  float3 albedo = color.rgb;
  float3 normal = normalMap.xyz;
  float ao = propMap.r;
  float roughness = propMap.g;
  float metalness = propMap.b;
  
  if(color.a < 0.5f)
  {
    t_outputMap[dtID.xy] = float4(1.0f, 1.0f, 1.0f, 0.0f);
    return;
  }
  
  normal = normal * 2.0f - 1.0f;
  float4 posWorld = depth;
  float3 viewDirection = normalize(viewPos.xyz - posWorld.xyz);
  float3 F0 = lerp(0.04, albedo, metalness);
  
  // === IBL ===
  float3 R = reflect(-viewDirection, normal);
  float nDotV = saturate(dot(normal, viewDirection));
  
  float3 diffuseIBL = getDiffuseLight(normal) * (albedo / PI);
  
  float3 specularMetal = getIBLRadianceGGX(normal, viewDirection, roughness);
  float3 metalFresnel = getIBLGGXFresnel(normal,
                                         viewDirection,
                                         roughness,
                                         F0,
                                         1.0f);
  metalFresnel *= specularMetal;
  
  float3 ambientLight = diffuseIBL + metalFresnel;
  
  // === Direct Lighting ===
  float3 lightDir = normalize(LightPos.xyz - posWorld.xyz);
  float NdL = saturate(dot(normal, lightDir));
  
  float3 specular = cookTorrenceSpecular(normal,
                                         viewDirection,
                                         lightDir,
                                         roughness,
                                         F0);
  
  float3 directLight = (specular + albedo) * NdL * lightIntensity;

  // === Shadows ===
  float4 lightWorldPos = mul(posWorld, mul(lightView, lightProj));
  float shadowFactor = shadowCalculation(lightWorldPos, NdL);
  
  float ssao = 1.0f;
  if (ssaoEnabled) {
    ssao = ssaoMap.r;
  }
  //float totalAO = clamp(ao + (1.0f - ssao), 0.0f, 1.0f);
  float totalAO = ao * ssao;
  
  //float3 finalColor = (ambientLight + directLight * shadowFactor) * ssao;
  //float3 emmisive = emmisiveMap.rgb;
  
  float3 ambient = ambientLight * totalAO;
  float3 direct = directLight * shadowFactor;
  float3 finalColor = ambient + direct;
  
  t_outputMap[dtID.xy] = float4(finalColor, 1.0f);
}
