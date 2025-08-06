#include "ShaderConstants.hlsl"

Texture2D<float4> t_envMap : register(t0);
RWTexture2D<float4> t_output : register(u0);

cbuffer PrefilterConstants : register(b2)
{
  uint width;
  uint height;
  uint samples;
  float roughness;
  float mipmapLevels;
  float3 pcPadding; // Padding to 16 bytes
};

// ----------------------------------------------------------------------------

[numthreads(32, 32, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  if (dtID.x >= width || dtID.y >= height)
  {
    return;
  }
  
  float2 texCoord = float2((dtID.x + 0.5f) / width, (dtID.y + 0.5f) / height);
  float px = t2p(texCoord.x, width);
  float py = t2p(texCoord.y, height);
  
  float3 normal = sphericalEnvMapToDirection(texCoord);
  float3x3 tbn = getNormalFrame(normal);
  float3 viewDir = normal;
  
  float3 result = float3(0.0f, 0.0f, 0.0f);
  float totalWeight = 0.0f;
  float alpha = roughness * roughness;
  
  // Get number of mips
  float adaptFactor = saturate(roughness + mipmapLevels / 8.0f);
  float maxBrightness = lerp(1.0f, 2.0f, adaptFactor);
  
  for (uint n = 0u; n < samples; ++n)
  {
    // GGX samples
    float3 rnd = random_pcg3d(uint3(uint(px), uint(py), n));
    float phi = 2.0f * PI * rnd.x;
    float u = rnd.y;
    
    float theta = acos(sqrt((1.0f - u) / (1.0f + (alpha * alpha - 1.0f) * u)));
    float3 posLocal = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
    float3 H = mul(posLocal, tbn);
    float3 L = 2.0f * dot(viewDir, H) * H - viewDir;
    
    float nDotL = dot(normal, L);
    if (nDotL > 0.0f)
    {
      float2 uv = directionToSphericalEnvMap(L);
      float3 radiance = pow(t_envMap.SampleLevel(samplerLinearClamp, uv, mipmapLevels).rgb, 1.0f);
      
      // === HDR brightness clamping ===
      float lum = dot(radiance, float3(0.2126f, 0.7152f, 0.0722f));
      float maxC = max(radiance.r, max(radiance.g, radiance.b));
      float brightness = max(lum, maxC * 0.5f);
      
      if (brightness < maxBrightness)
      {
        radiance *= (maxBrightness / brightness);
      }
      
      result += radiance * nDotL;
      totalWeight += nDotL;
    }
  }

  result = (totalWeight > 0.0f) ? (result / totalWeight) : float3(0.0f, 0.0f, 0.0f);
  //result /= PI;
  //result *= 11.0f;
  t_output[dtID.xy] = float4(result, 1.0f);
}
