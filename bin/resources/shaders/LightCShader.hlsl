#include "ShaderConstants.hlsl"

Texture2D t_depthMap : register(t0);
Texture2D t_normalMap : register(t1);
Texture2D t_colorMap : register(t2);
Texture2D t_propMap : register(t3);
Texture2D t_aoMap : register(t4);
Texture2D t_shadowMap : register(t5);
Texture2D t_depthStencil : register(t6);
Texture2D t_skybox : register(t7);
//Texture2DArray<float4> t_diffIrrCube : register(t7);
//Texture2DArray<float4> t_specPreCube : register(t8);
Texture2D t_brdfLUT : register(t8);
RWTexture2D<float4> t_outputMap : register(u0);

#ifndef PCF_KERNEL_SIZE
#define PCF_KERNEL_SIZE 5
#endif
#ifndef DELTA
#define DELTA 0.00000001
#endif

#define SAMPLE_DELTA 0.2f
#define MAX_REFLECTION_LOD 5.0f

cbuffer Light : register(b2)
{
  float4 LightPos[12];
}

cbuffer LightCam : register(b3)
{
  float4x4 lightView;
  float4x4 lightProj;
}

cbuffer skyboxConstants : register(b4)
{
  float4x4 matSkyRotation;
}

//struct PS_INPUT
//{
//  float4 Position : SV_POSITION;
//  float2 Texcoord : TEXCOORD0;
//};

//struct BRDFInput
//{
//  float3 viewDir;
//  float3 normal;
//  float3 lightDir;
//  
//  float nDotH;
//  float nDotL;
//  float nDotV;
//  float F0;
//  float sigmaSqrd;
//  float halfSigmaSqrd;
//  
//  float3 specularColor;
//  float4 albedo;
//  float metallic;
//  float roughness;
//  
//  bool bHasSpecularPath;
//};

//struct BRDFOutput
//{
//  float3 diffuse;
//  float3 specular;
//  float3 fresnel;
//  //float3 ambient;
//  //float shadowFactor;
//};

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
                            float metallic,
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
  
  //float kS = F;
  //float kD = float3(1.0f) - kS;
  //kD *= (1.0f - metallic);

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

float3
getDiffuseIrradiance(float3 normal)
{
  float3 irradiance = float3(0.0f, 0.0f, 0.0f);
  
  float3 up = float3(0.0f, 1.0f, 0.0f);
  float3 right = normalize(cross(up, normal));
  up = normalize(cross(normal, right));
  
  float nrSamples = 0.0f;
  for (float phi = 0.0f; phi < PI * 2.0f; phi += SAMPLE_DELTA)
  {
    for (float theta = 0.0f; theta < PI * 0.5f; theta += SAMPLE_DELTA)
    {
      // Spherical coordinates to Cartesian (in tangential space)
      float3 tangentSample = float3(sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi));
      // Convert to world space using the tangent basis
      //float3 sampleVec = tangentSample.x * right + tangentSample.y * up +
      //                   tangentSample.z * normal;
      float2 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
      
      irradiance += t_skybox.SampleLevel(samplerLinearClamp, sampleVec, 0).rgb *
                    cos(theta) * sin(theta);
      
      ++nrSamples;
    }
  }
  irradiance *= PI / nrSamples;
  
  return irradiance;
}

//float
//ndf_GGX(float nDotH, float roughness, float alpha)
//{
//  float alphaSqrd = alpha * alpha;
//  float cos2Theta = nDotH * nDotH;
//  float tan2Theta = (1.0f - cos2Theta) / (cos2Theta + 1e-5f);
//  
//  return alphaSqrd / (PI * cos2Theta * pow((alphaSqrd + tan2Theta), 2.0f));
//}

//BRDFOutput
//BRDF(in BRDFInput inData)
//{
//  BRDFOutput outData = (BRDFOutput) 0;
  
//  float halfDotV = dot(normalize(inData.viewDir + inData.lightDir), inData.normal);
  
//  float3 F = fresnelSchlick(inData.F0, halfDotV);
//  float D = ndf_GGX(inData.nDotH, inData.roughness, inData.sigmaSqrd);
//  float G = geometrySmith(inData.nDotV, inData.nDotL, inData.roughness);
//  float3 specular = (D * G * F) / max(DELTA, 4.0f * inData.nDotV * inData.nDotL);
//  specular *= inData.bHasSpecularPath ? inData.specularColor : 1.0f;
  
//  // Diffuse with energy conservation
//  outData.diffuse = Lambert(F, inData.albedo.rgb, inData.metallic);
//  outData.specular = specular;
//  outData.fresnel = F;
  
//  return outData;
//}

//float4 getSpecularSample(float3 reflection, float lod)
//{
//  float u_EnvIntensity = 1.0f; // Environment intensity, can be adjusted
  
//  float2 uv = getSkyBoxUV(normalize(mul(float4(reflection, 0.0f), matSkyRotation).xyz));
//  // Sample the texture at the specified LOD level
//  float4 texSample = t_skyReflect.SampleLevel(samplerLinearClamp, uv, lod);
  
//  texSample.rgb *= u_EnvIntensity; // Apply environment intensity
//  return float4(texSample.rgb, 1.0f);
//}

//float3 getIBLRadianceGGX(float3 n, float3 v, float roughness)
//{
//  float NdotV = clampedDot(n, v);
  
//  float3 texDimensions;
//  t_skyReflect.GetDimensions(0, texDimensions.x, texDimensions.y, texDimensions.z);
//  float lod = min((roughness * (texDimensions.x - 1)) + 1.0f, texDimensions.z);
  
//  float3 reflection = normalize(reflect(-v, n));
//  float4 specularSample = getSpecularSample(reflection, lod);
  
//  return specularSample.rgb;
//}

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
  //float depthStencil = t_depthStencil.Load(int3(dtID.xy, 0));
  
  float3 albedo = color.rgb;
  float metalness = propMap.r;
  float3 normal = normalMap.xyz;
  float roughness = propMap.b;
  
  if(color.a < 0.5f)
  {
    t_outputMap[dtID.xy] = float4(1.0f, 1.0f, 1.0f, 0.0f);
    return;
  }
  //if (normalMap.w == 0)
  //{
  //  t_outputMap[dtID.xy] = float4(1.0f, 1.0f, 1.0f, 0.0f);
  //  return;
  //}
  
  //if (depthStencil >= 0.9999f)
  //{
  //  color = t_skyMap.Load(uint3(dtID.xy, 0));
  //  t_outputMap[dtID.xy] = float4(color.rgb, 1.0f);
  //  return;
  //}
  
  normal = normal * 2.0f - 1.0f;
  float4 posWorld = depth;
  float3 viewDirection = normalize(viewPos.xyz - posWorld.xyz);
  float3 F0 = lerp(0.04, albedo, metalness);
  
  // === IBL ===
  float3 R = reflect(-viewDirection, normal);
  float nDotV = saturate(dot(normal, viewDirection));
  
  //float3 irradiance = t_diffIrrCube.SampleLevel(samplerLinearClamp, normal, 0).rgb;
  float3 irradiance = getDiffuseIrradiance(normal);
  float3 diffuseIBL = irradiance * albedo;
  
  //float3 prefilteredColor = t_specPreCube.SampleLevel(samplerLinearClamp,
  //                                                    R,
  //                                                    roughness * MAX_REFLECTION_LOD).rgb;
  //float2 brdf = t_brdfLUT.SampleLevel(samplerLinearClamp, float2(nDotV, roughness), 0).rg;
  //float3 specularIBL = prefilteredColor * (F0 * brdf.x + brdf.y);
  
  float3 ambientLight = diffuseIBL; //+ specularIBL;
  
  // === Direct Lighting ===
  float3 lightDir = normalize(LightPos[0].xyz - posWorld.xyz);
  float NdL = saturate(dot(normal, lightDir));
  
  float3 specular = cookTorrenceSpecular(normal,
                                         viewDirection,
                                         lightDir,
                                         roughness,
                                         metalness,
                                         F0);
  
  float3 directLight = (specular + albedo) * NdL * lightIntensity;
  //float3 finalColor = ((((diffuse + specular) * lightIntensity * NdL) + ambientLight) * ao.r);

  // === Shadows ===
  float4 lightWorldPos = mul(posWorld, lightView);
  lightWorldPos = mul(lightWorldPos, lightProj);
  lightWorldPos.xyz /= lightWorldPos.w;
  lightWorldPos.xyz = lightWorldPos.xyz * 0.5f + 0.5f;
  
  //float2 shadowCoord = lightWorldPos.xy;
  //shadowCoord.y = 1.0f - shadowCoord.y;
  //float shadowFactor = 1.0f;
  //if(shadowCoord.x < 0.0f || shadowCoord.x > 1.0f ||
  //   shadowCoord.y < 0.0f || shadowCoord.x > 1.0f) {
  //  t_outputMap[dtID.xy] = float4(ambientLight, 1.0f);
  //  return;
  //}
  
  float2 shadowCoord = float2(lightWorldPos.x, 1.0f - lightWorldPos.y);
  float shadowFactor = 1.0f;
  float shadowBias = max(0.001f * (1.0f - NdL), 0.001f);
  float texelSize = 1.0f / shadowMapSize;
  
  if (shadowCoord.x < 0.0f || shadowCoord.x > 1.0f ||
      shadowCoord.y < 0.0f || shadowCoord.y > 1.0f)
  {
    shadowFactor = 0.0f;
  }
  else
  {
    shadowFactor = pcFiltering(shadowCoord, lightWorldPos.z, texelSize, shadowBias);
  }
  
  //shadowFactor = pcFiltering(shadowCoord, lightWorldPos.z, texelSize, shadowBias);
  //finalColor *= shadowFactor;
  
  float3 finalColor = (ambientLight + directLight * shadowFactor) * ao.r;
  
  t_outputMap[dtID.xy] = float4(finalColor, 1.0f);
}