#include "ShaderConstants.hlsl"

Texture2DArray<float4> t_envCube : register(t0);
RWTexture2DArray<float4> t_output : register(u0);

#define SAMPLE_COUNT 1024
#define CUBEMAP_SIZE 1024

float3
texCoordToDir(uint faceIndex, float2 uv)
{
  uv = uv * 2.0f - 1.0f; // Map from [0,1] to [-1,1]

  float3 dir = 0;
  if (faceIndex == 0)
  {
    dir = float3(1.0, -uv.y, -uv.x);
  }
  else if (faceIndex == 1)
  {
    dir = float3(-1.0, -uv.y, uv.x);
  }
  else if (faceIndex == 2)
  {
    dir = float3(uv.x, 1.0, uv.y);
  }
  else if (faceIndex == 3)
  {
    dir = float3(uv.x, -1.0, -uv.y);
  }
  else if (faceIndex == 4)
  {
    dir = float3(uv.x, -uv.y, 1.0);
  }
  else if (faceIndex == 5)
  {
    dir = float3(-uv.x, -uv.y, -1.0);
  }
  
  return normalize(dir);
}

void
directionToCubeUV(float3 dir, out uint faceIndex, out float2 uv)
{
  float3 absDir = abs(dir);
  
  if (absDir.x > absDir.y && absDir.x > absDir.z)
  {
    if (dir.x > 0)
    {
      // +X
      faceIndex = 0;
      uv = float2(-dir.z, -dir.y) / absDir.x;
    }
    else
    {
      // -X
      faceIndex = 1;
      uv = float2(dir.z, -dir.y) / absDir.x;
    }
  }
  else if (absDir.y > absDir.x && absDir.y > absDir.z)
  {
    if (dir.y > 0)
    {
      // +Y
      faceIndex = 2;
      uv = float2(dir.x, dir.z) / absDir.y;
    }
    else
    {
      // -Y
      faceIndex = 3;
      uv = float2(dir.x, -dir.z) / absDir.y;
    }
  }
  else
  {
    if (dir.z > 0)
    {
      // +Z
      faceIndex = 4;
      uv = float2(dir.x, -dir.y) / absDir.z;
    }
    else
    {
      // -Z
      faceIndex = 5;
      uv = float2(-dir.x, -dir.y) / absDir.z;
    }
  }
  
  uv = (uv * 0.5f + 0.5f); // Map from [-1,1] to [0,1]
}

// ----------------------------------------------------------------------------

[numthreads(32, 32, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  float faceIndex = dtID.z;
  float2 uv = (dtID.xy + 0.5f) / CUBEMAP_SIZE; // Convert to [0,1] range
  
  float3 N = normalize(texCoordToDir(faceIndex, uv));
  float3 R = N;
  float3 V = R;
  
  float totalWeight = 0.0f;
  float3 prefilteredColor = float3(0.0f, 0.0f, 0.0f);
  
  for(uint i = 0; i < SAMPLE_COUNT; ++i) {
    float2 Xi = hammersley(i, SAMPLE_COUNT);
    float3 H = importanceSampleGGX(Xi, N, 0.5f); // Roughness is set to 0.5 for this example
    float3 L = normalize(2.0f * dot(V, H) * H - V);
    
    float NdotL = max(dot(N, L), 0.0f);
    if (NdotL > 0.0f) {
      float2 sampleUV;
      uint sampleFaceIndex;
      directionToCubeUV(L, sampleFaceIndex, sampleUV);
      float3 sampleColor = t_envCube.Load(int4(sampleUV * CUBEMAP_SIZE, sampleFaceIndex, 0)).rgb;
      prefilteredColor += sampleColor * NdotL;
      
      //prefilteredColor += t_envCube.SampleLevel(samplerLinearClamp, L, 0).rgb * NdotL;
      totalWeight += NdotL;
    }
  }
  
  prefilteredColor /= totalWeight;
  
  t_output[uint3(dtID.xy, uint(faceIndex))] = float4(prefilteredColor, 1.0f);
}
