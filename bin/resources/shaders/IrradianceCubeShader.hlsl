#include "ShaderConstants.hlsl"

Texture2D t_skybox : register(t0);
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

[numthreads(16, 16, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  if(dtID.x >= width || dtID.y >= height) {
    return;
  }
  
  float2 texCoord = float2((dtID.x + 0.5f) / width, (dtID.y + 0.5f) / height);
  float px = t2p(texCoord.x, width);
  float py = t2p(texCoord.y, height);
  
  float3 normal = sphericalEnvMapToDirection(texCoord);
  float3x3 tbn = getNormalFrame(normal);
  
  float3 result = float3(0.0f, 0.0f, 0.0f);
  float totalWeight = 0.0f;
  
  const float maxBrightness = 30.0f;
  
  for(uint n = 0; n < samples; ++n) {
    float3 rnd = random_pcg3d(uint3(uint(px), uint(py), n));
    float phi = 2.0f * PI * rnd.x;
    float cosTheta = sqrt(1.0f - rnd.y);
    float sinTheta = sqrt(1.0f - cosTheta * cosTheta);
    
    float3 posLocal = float3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
    float3 L = mul(posLocal, tbn);
    
    float ndotL = saturate(dot(normal, L));
    if(ndotL > 0.0f) {
      float2 uv = directionToSphericalEnvMap(L);
      float3 radiance = pow(t_skybox.SampleLevel(samplerLinearClamp, uv,
                                                 mipmapLevels).rgb, 1.0f);
      float brightness = max(radiance.r, max(radiance.g, radiance.b));
      if(brightness > maxBrightness) {
        radiance *= maxBrightness / brightness;
      }
      
      result += radiance * ndotL;
      totalWeight += ndotL;
    }
  }
  
  result = (totalWeight > 0.0f) ? (result / totalWeight) : 0.0f;
  result /= PI;
  result *= 11.0f;
  t_output[dtID.xy] = float4(result, 1.0f);
}
