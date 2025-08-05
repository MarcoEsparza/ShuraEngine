#include "ShaderConstants.hlsl"

Texture2DArray<float4> t_skybox : register(t0);
RWTexture2DArray<float4> t_outputMap : register(u0);

#define SAMPLE_DELTA 0.025f
#define CUBEMAP_SIZE 1024

//float3 getCubeMapDirection(float u, float v, uint face)
//{
//    // Remap [0,1] -> [-1,1]
//  float x = 2.0f * u - 1.0f;
//  float y = 2.0f * v - 1.0f;

//  float3 dir;
  
//  if (face == 0) {
//    dir = float3(1.0f, -y, -x);
//  }
//  else if (face == 1) {
//    dir = float3(-1.0f, -y, x);
//  }
//  else if(face == 2) {
//    dir = float3(x, 1.0f, y);
//  }
//  else if(face == 3) {
//    dir = float3(x, -1.0f, -y);
//  }
//  else if(face == 4) {
//    dir = float3(x, -y, 1.0f);
//  }
//  else if (face == 5) {
//    dir = float3(-x, -y, -1.0f);
//  }

//  return normalize(dir);
//}

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
      float3 sampleVec = tangentSample.x * right + tangentSample.y * up +
                         tangentSample.z * normal;
      
      //irradiance += t_skyReflect.SampleLevel(samplerLinearClamp, sampleVec, 0).rgb *
      //              cos(theta) * sin(theta);
      
      irradiance += t_skybox.Load(uint4(sampleVec.xyz, 0)).rgb * cos(theta) * sin(theta);
      
      ++nrSamples;
    }
  }
  irradiance *= PI / nrSamples;
  
  return irradiance;
}

//[numthreads(32, 32, 1)]
//void
//CSMain(uint3 dtID : SV_DispatchThreadID)
//{
//  uint x = dtID.x;
//  uint y = dtID.y;
//  uint face = dtID.z;
  
//  if (x >= CUBEMAP_SIZE || y >= CUBEMAP_SIZE || face >= 6) {
//    return;
//  }
  
//  float u = (float(x) + 0.5f) / CUBEMAP_SIZE;
//  float v = (float(y) + 0.5f) / CUBEMAP_SIZE;
  
//  //float3 N = getCubeMapDirection(u, v, face);
//  float3 N = texCoordToDir(face, float2(u, v));
//  float3 irradiance = getDiffuseIrradiance(N);
  
//  t_outputMap[uint3(x, y, face)] = float4(irradiance, 1.0f);
//}

[numthreads(32, 32, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  uint x = dtID.x;
  uint y = dtID.y;
  uint face = dtID.z;
  
  if (x >= CUBEMAP_SIZE || y >= CUBEMAP_SIZE || face >= 6)
  {
    return;
  }
  
  float u = (float(x) + 0.5f) / CUBEMAP_SIZE;
  float v = (float(y) + 0.5f) / CUBEMAP_SIZE;
  
  //float3 N = getCubeMapDirection(u, v, face);
  float3 N = texCoordToDir(face, float2(u, v));
  float3 irradiance = getDiffuseIrradiance(N);
  
  t_outputMap[uint3(x, y, face)] = float4(irradiance, 1.0f);
}
