#include "ShaderConstants.hlsl"

RWTexture2D<float4> BRDFResult : register(u0);

//-----------------------------------------------------------------------------
// LUT compute functions used by IblBrdf.hlsl                                  
//-----------------------------------------------------------------------------
// Geometry term
// http://graphicrants.blogspot.com.au/2013/08/specular-brdf-reference.html
// I could not have arrived at this without the notes at :
// http://www.gamedev.net/topic/658769-ue4-ibl-glsl/

float GGX(float NoV, float roughness)
{
    // http://graphicrants.blogspot.com.au/2013/08/specular-brdf-reference.html
    // Schlick-Beckmann G.
  float k = roughness / 2.0f;
  return NoV / (NoV * (1.0f - k) + k);
}

float geometryForLut(float roughness, float NoL)
{
  return GGX(NoL, roughness * roughness);
}

// Visibility term
float visibilityForLut(float roughness, float NoV)
{
  return GGX(NoV, roughness * roughness);
}

// Fresnel Term.
// Inputs, view dot half angle.
float fresnelForLut(float VoH)
{
  return pow(1.0f - VoH, 5.0f);
}

// Summation of Lut term while iterating over samples
float2 sumLut(float2 current, float G, float V, float F,
              float VoH, float NoL, float NoH, float NoV)
{
  G = G * V;
  float G_Vis = G * VoH / (NoH * NoV);
  current.x += (1.0f - F) * G_Vis;
  current.y += F * G_Vis;

  return current;
}

float2
integrate(float roughness, float NoV)
{
  float3 N = float3(0.0f, 0.0f, 1.0f);
  float3 V = float3(sqrt(1.0f - NoV * NoV), 0.0f, NoV);
  float2 result = float2(0.0f, 0.0f);

  const uint NumSamples = 1024;

  precise float Vis = visibilityForLut(roughness, NoV);

  for (uint i = 0; i < NumSamples; i++)
  {
    float2 Xi = hammersley(i, NumSamples);
    float3 H = importanceSampleGGX(Xi, roughness, N);
    precise float3 L = 2.0f * dot(V, H) * H - V;

    float NoL = saturate(L.z);
    float NoH = saturate(H.z);
    float VoH = saturate(dot(V, H));
    float NdV = saturate(dot(N, V));
    if (NoL > 0.0f)
    {
      precise float G = geometryForLut(roughness, NoL);
      precise float F = fresnelForLut(VoH);
      result = sumLut(result, G, Vis, F, VoH, NoL, NoH, NdV);
    }
  }

  result.x = (result.x / float(NumSamples));
  result.y = (result.y / float(NumSamples));

  return result;
}

[numthreads(16, 16, 1)]
void CSMain(uint2 id : SV_DispatchThreadID)
{
  float roughness = (float) (id.y + 0.5f) / 256.0f;
  float NoV = (float) (id.x + 0.5f) / 256.0f;
      
  float2 result = integrate(roughness, NoV);
    // [TODO]: I need to fix my DDS saver so that is can save R32G32 and R16G16
  BRDFResult[int2(id.x, 255 - id.y)] = float4(result.x, result.y, roughness, 1.0f);
}
