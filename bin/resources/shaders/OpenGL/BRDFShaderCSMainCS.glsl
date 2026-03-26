#version 430 core

#include "ShaderConstants.glsl"

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
layout(binding = 0, rgba16f) uniform image2D lutImage;

float GGX(float NoV, float roughness)
{
  float k = roughness / 2.0;
  return NoV / (NoV * (1.0 - k) + k);
}

float geometryForLut(float roughness, float NoL)
{
  return GGX(NoL, roughness * roughness);
}

float visibilityForLut(float roughness, float NoV)
{
  return GGX(NoV, roughness * roughness);
}

float fresnelForLut(float VoH)
{
  return pow(1.0 - VoH, 5.0);
}

vec2 sumLut(vec2 current, float G, float V, float F, float VoH, float NoL, float NoH, float NoV)
{
  G *= V;
  float G_Vis = G * VoH / (NoH * NoV);
  current.x += (1.0 - F) * G_Vis;
  current.y += F * G_Vis;
  return current;
}

vec2 integrate(float roughness, float NoV)
{
  vec3 N = vec3(0.0, 0.0, 1.0);
  vec3 V = vec3(sqrt(1.0 - NoV * NoV), 0.0, NoV);
  vec2 result = vec2(0.0, 0.0);

  const uint SAMPLE_COUNT = 1024u;

  float Vis = visibilityForLut(roughness, NoV);

  for (uint i = 0u; i < SAMPLE_COUNT; ++i)
  {
    vec2 Xi = hammersley(i, SAMPLE_COUNT);
    vec3 H = importanceSampleGGX(Xi, roughness, N);
    vec3 L = normalize(2.0 * dot(V, H) * H - V);

    float NoL = clamp(L.z, 0.0, 1.0);
    float NoH = clamp(H.z, 0.0, 1.0);
    float VoH = clamp(dot(V, H), 0.0, 1.0);
    float NdV = clamp(dot(N, V), 0.0, 1.0);

    if (NoL > 0.0)
    {
      float G = geometryForLut(roughness, NoL);
      float F = fresnelForLut(VoH);
      result = sumLut(result, G, Vis, F, VoH, NoL, NoH, NdV);
    }
  }

  result.x /= float(SAMPLE_COUNT);
  result.y /= float(SAMPLE_COUNT);

  return result;
}

void main()
{
  float roughness = gl_GlobalInvocationID.y + 0.5 / 256.0;
  float NoV = gl_GlobalInvocationID.x + 0.5 / 256.0;

  vec2 integratedBRDF = integrate(roughness, NoV);
  imageStore(lutImage, ivec2(gl_GlobalInvocationID.xy), vec4(integratedBRDF, 0.0, 1.0));
}
