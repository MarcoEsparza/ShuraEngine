#include "ShaderConstants.hlsl"

Texture2D t_skybox : register(t0);
//Texture2DArray<float4> t_skybox : register(t0);
//TextureCube<float4> t_skybox : register(t0);
RWTexture2D<float4> t_outputMap : register(u0);

//#ifndef PI
//#define PI 3.14159265359
//#endif
//#ifndef RECIPROCAL_PI
//#define RECIPROCAL_PI 1.0f / 3.14159265359
//#endif
//#ifndef RECIPROCAL_2PI
//#define RECIPROCAL_2PI 1.0f / (2 * 3.14159265359)
//#endif

//float2
//getSkyBoxUV(float3 dir)
//{
//  float u = -atan2(dir.z, dir.x) * RECIPROCAL_2PI + 0.5f;
//  float v = acos(dir.y) * RECIPROCAL_PI;
//  return float2(u, v);
//}

float2
equirectUV(float3 dir)
{
  float u = atan2(dir.z, dir.x) * RECIPROCAL_2PI + 0.5;
  float v = acos(clamp(dir.y, -1.0, 1.0)) * RECIPROCAL_PI;
  return float2(u, v);
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

  
  //if (absDir.x >= absDir.y && absDir.x >= absDir.z) {
  //  faceIndex = (dir.x > 0.0f) ? 0 : 1; // +X or -X
  //  uv = float2((dir.z / absDir.x + 1.0f) * 0.5f, (-dir.y / absDir.x + 1.0f) * 0.5f);
  //}
  //else if (absDir.y >= absDir.x && absDir.y >= absDir.z) {
  //  faceIndex = (dir.y > 0.0f) ? 2 : 3; // +Y or -Y
  //  uv = float2((dir.x / absDir.y + 1.0f) * 0.5f, (-dir.z / absDir.y + 1.0f) * 0.5f);
  //}
  //else {
  //  faceIndex = (dir.z > 0.0f) ? 4 : 5; // +Z or -Z
  //  uv = float2((-dir.x / absDir.z + 1.0f) * 0.5f, (-dir.y / absDir.z + 1.0f) * 0.5f);
  //}
  
  uv = (uv * 0.5f + 0.5f); // Map from [-1,1] to [0,1]
}

[numthreads(32, 32, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  float2 ndc = dtID.xy / screenSize * 2.0f - 1.0f;
  ndc.y = -ndc.y;
  float4 clipSpacePos = float4(ndc, 1.0f, 1.0f);
  
  matrix newViewMatrix = matView;
  newViewMatrix[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
  
  float4 pos = mul(clipSpacePos, newViewMatrix);
  pos = float4(pos.xyz, 1.0f);
  
  pos = mul(pos, matProjectionTranspose);
  pos.z = pos.w;
  
  float3 dir = normalize(pos.xyz);
  float2 skyUV = getSkyBoxUV(dir);
  float3 color = t_skybox.SampleLevel(samplerLinearClamp, skyUV, 0).xyz;

  t_outputMap[dtID.xy] = float4(color, 1.0f);
}

//[numthreads(32, 32, 1)]
//void
//CSMain(uint3 dtID : SV_DispatchThreadID)
//{
//  float2 ndc = dtID.xy / screenSize * 2.0f - 1.0f;
//  ndc.y = -ndc.y;
//  float4 clipSpacePos = float4(ndc, 1.0f, 1.0f);
  
//  //float4 viewDir = mul(clipSpacePos, matViewProjInverseTranspose);
//  //viewDir.xyz /= viewDir.w; // Perspective divide
//  //float3 dir = normalize(viewDir.xyz);
  
//  matrix newViewMatrix = matViewTranspose;
//  newViewMatrix[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
//  float4 pos = mul(clipSpacePos, newViewMatrix);
//  pos = float4(pos.xyz, 1.0f);
//  pos = mul(pos, matProjectionTranspose);
//  pos.z = pos.w;
//  float3 dir = normalize(pos.xyz);
  
//  uint faceIndex;
//  float2 uv;
//  directionToCubeUV(dir, faceIndex, uv);
  
//  float3 color = t_skybox.SampleLevel(samplerLinearClamp, float3(uv, faceIndex), 0).xyz;
//  //float3 color = t_skybox.SampleLevel(samplerLinearClamp, dir, 0).xyz;
  
//  t_outputMap[dtID.xy] = float4(color, 1.0f);
//}
