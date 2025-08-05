#include "ShaderConstants.hlsl"

RWTexture2D<float4> t_brdfLUT : register(u0);

#define SAMPLE_COUNT 1024

// ----------------------------------------------------------------------------

float geometrySchlick(float nDotV, float roughness)
{
  float r = roughness + 1.0f;
  float k = (r * r) / 8.0f;
  float ik = 1.0f - k;
  return nDotV / (nDotV * ik + k);
}

// ----------------------------------------------------------------------------

float geometrySmith(float nDotV, float nDotL, float roughness)
{
  float ggx1 = geometrySchlick(nDotV, roughness);
  float ggx2 = geometrySchlick(nDotL, roughness);
  return ggx1 * ggx2;
}

// ----------------------------------------------------------------------------

float
GGX(float NoV, float roughness)
{
  float k = roughness / 2.0f;
  return NoV / (NoV * (1.0f - k) + k);
}

float
geometryForLUT(float roughness, float NoL)
{
  return GGX(NoL, roughness * roughness);
}

float
visibilityForLUT(float roughness, float NoV)
{
  return GGX(NoV, roughness * roughness);
}

float2
sumLUT(float2 current, float G, float V, float F, float VoH, float NoL, float NoH, float NoV)
{
  G = G * V;
  float G_Vis = (G * VoH) / (NoH + NoV);
  current.x += (1.0f - F) * G_Vis;
  current.y += F * G_Vis;
  
  return current;
}

float
fresnelForLUT(float VoH)
{
  // Schlick's approximation
  return pow(1.0f - VoH, 5.0f);
}

float2
integrateBRDF(float nDotV, float roughness)
{
  float3 N = float3(0.0f, 0.0f, 1.0f);
  float3 V;
  V.x = sqrt(1.0f - nDotV * nDotV);
  V.y = 0.0f;
  V.z = nDotV;
  
  float2 result = float2(0.0f, 0.0f);
  float vis = visibilityForLUT(roughness, nDotV);
  
  for (uint i = 0; i < SAMPLE_COUNT; ++i)
  {
    float2 Xi = hammersley(i, SAMPLE_COUNT);
    float3 H = importanceSampleGGX(Xi, N, roughness);
    precise float3 L = 2.0f * dot(V, H) * H - V;
    
    float nDotL = saturate(L.z);
    float nDotH = saturate(H.z);
    float vDotH = saturate(dot(V, H));
    float nDV = saturate(dot(N, V));
    
    if (nDotL > 0.0f)
    {
      float G = geometryForLUT(roughness, nDotL);
      float F = fresnelForLUT(vDotH);
      result = sumLUT(result, G, vis, F, vDotH, nDotL, nDotH, nDV);
    }
  }

  result.x /= float(SAMPLE_COUNT);
  result.y /= float(SAMPLE_COUNT);
  
  return result;
}



// ----------------------------------------------------------------------------

[numthreads(32, 32, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  uint2 dimensions;
  t_brdfLUT.GetDimensions(dimensions.x, dimensions.y);

  float roughness = (float(dtID.y) + 0.5f) / float(dimensions.y);
  float nDotV = (float(dtID.x) + 0.5f) / float(dimensions.x);
  
  //float2 uv = float2(dtID.xy) / float2(dimensions);
  //float nDotV = uv.x;
  //float roughness = uv.y;
  
  float2 integratedBRDF = integrateBRDF(nDotV, roughness);
  t_brdfLUT[dtID.xy] = float4(integratedBRDF, 0.0f, 1.0f);
}
