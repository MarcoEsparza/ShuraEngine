SamplerState samplerLinearWrap : register(s0);
SamplerState samplerPointWrap : register(s1);
SamplerState samplerAnisotropicWrap : register(s2);
SamplerState samplerLinearClamp : register(s3);
SamplerState samplerPointClamp : register(s4);
SamplerState samplerAnisotropicClamp : register(s5);

#define PI 3.14159265358979323f
#define RECIPROCAL_PI 1.0f / PI
#define RECIPROCAL_2PI 1.0f / (2.0f * PI)
#define INV_PI 0.31830988618379067239521257108191f

// ----------------------------------------------------------------------------

cbuffer MainBuffer : register(b0)
{
  // Camera view
  float4x4 matView;
  float4x4 matViewTranspose;
  float4x4 matViewInverse;
  float4x4 matViewInverseTranspose;

  // Camera projection
  float4x4 matProjection;
  float4x4 matProjectionTranspose;
  float4x4 matProjectionInverse;
  float4x4 matProjectionInverseTranspose;

  float4x4 matViewProjInverse;
  float4x4 matViewProjInverseTranspose;
  
  // Viewport
  float2 screenSize;
  float nearClip;
  float farClip;

  // Camera data
  float4 viewPos;
  float4 viewTarget;

  // Time
  float time;
  float deltaTime;
  float cosTime;
  float sinTime;
}

// ----------------------------------------------------------------------------

cbuffer ShaderData : register(b1)
{
  // Ambient occlusion settings
  float sampleRad;
  float aoScale;
  float aoBias;
  float aoIntensity;
    
  // ShadowMap
  float shadowMapSize;
    
  // Tone mapping settings
  float toneMapIndex;
  float lutSize;
  float whitePoint;
  float bloomMultiplier;
  float brightThreshold;
    
  // Post-processing settings
  float minR;
  float maxR;
  float minG;
  float maxG;
  float minB;
  float maxB;
    
  // Mipmap settings
  float mipLevel0;
  float mipLevel1;
  
  float ssaoEnabled; // 1 if SSAO is enabled, 0 otherwise
  float middleGrey; // Middle grey value for tone mapping
  
  float cubeRoughness; // Roughness value for environment mapping
  uint cubeFaceIndex; // Face index for environment mapping
  float emmisiveIntensity; // Emissive intensity multiplier
  float sdPadding; // Shader data padding
}

struct MaterialProperties
{
  bool bHasBaseColor; // true if base color texture is used
  bool bHasSpecular; // true if specular texture is used
  bool bHasNormalMap; // true if normal map is used
  bool bHasMetallicMap; // true if metallic map is used
  bool bHasRoughnessMap; // true if roughness map is used
  bool bHasAmbientOcclusion; // true if ambient occlusion map is used
  bool bHasEmissiveMap; // true if emissive map is used
  bool bIsOpaque; // true if material is opaque
  bool bHasAlphaTest; // true if material uses alpha test
  bool bHasAlphaBlend; // true if material uses alpha blending
  bool bIsDoubleSided; // true if material is double sided
  bool bWireframeEnabled; // true if wireframe rendering is enabled
  bool bCanCastShadow; // true if material can cast shadows
  bool bCanReceiveShadow; // true if material can receive shadows
  bool bInvertRoughness; // true if roughness should be inverted
};

// ----------------------------------------------------------------------------

MaterialProperties
getMaterialProperties(int materialProps)
{
  MaterialProperties props = (MaterialProperties) 0;
  props.bHasBaseColor = (materialProps & 1) == 1; // bit 0
  props.bHasSpecular = (materialProps & 2) == 2; // bit 1
  props.bHasNormalMap = (materialProps & 4) == 4; // bit 2
  props.bHasMetallicMap = (materialProps & 8) == 8; // bit 3
  props.bHasRoughnessMap = (materialProps & 16) == 16; // bit 4
  props.bHasAmbientOcclusion = (materialProps & 32) == 32; // bit 5
  props.bHasEmissiveMap = (materialProps & 64) == 64; // bit 6
  props.bIsOpaque = (materialProps & 128) == 128; // bit 7
  props.bHasAlphaTest = (materialProps & 256) == 256; // bit 8
  props.bHasAlphaBlend = (materialProps & 512) == 512; // bit 9
  props.bIsDoubleSided = (materialProps & 1024) == 1024; // bit10
  props.bWireframeEnabled = (materialProps & 2048) == 2048; // bit11
  props.bCanCastShadow = (materialProps & 4096) == 4096; // bit12
  props.bCanReceiveShadow = (materialProps & 8192) == 8192; // bit13
  props.bInvertRoughness = (materialProps & 16384) == 16384; // bit14
    
  return props;
}

// ----------------------------------------------------------------------------

float4
sRGBToLinear(float4 color)
{
  // Convert sRGB to linear color space
  return float4(pow(color.rgb, 1.0f / 2.2f), color.a);
}

float4
linearToSRGB(float4 color)
{
  // Convert linear to sRGB color space
  return float4(pow(color.rgb, 2.2f), color.a);
}

// ----------------------------------------------------------------------------

float2
getSkyBoxUV(float3 dir)
{
  float u = atan2(dir.z, dir.x) * RECIPROCAL_2PI + 0.5f;
  float v = acos(dir.y) * RECIPROCAL_PI;
  return float2(u, v);
}

// ----------------------------------------------------------------------------

float clampedDot(float3 a, float3 b)
{
  return max(0.0f, dot(a, b));
}

// ----------------------------------------------------------------------------

float
radicalInverse_VdC(uint bits)
{
  bits = (bits << 16u) | (bits >> 16u);
  bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
  bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
  bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
  bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
  return float(bits) * 2.3283064365386963e-10f; // / 0x100000000
}

// ----------------------------------------------------------------------------

float2
hammersley(uint i, uint N)
{
  return float2(float(i) / float(N), radicalInverse_VdC(i));
}

// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Shader functions used by IblImportanceSamplingDiffuse.fx                    
//-----------------------------------------------------------------------------
//
// Derived from GGX example in:
// http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
// Image Based Lighting.
//
float3 importanceSampleDiffuse(float2 Xi, float3 N)
{
  float CosTheta = 1.0f - Xi.y;
  float SinTheta = sqrt(1.0f - CosTheta * CosTheta);
  float Phi = 2.0f * PI * Xi.x;

  float3 H;
  H.x = SinTheta * cos(Phi);
  H.y = SinTheta * sin(Phi);
  H.z = CosTheta;

  float3 UpVector = abs(N.z) < 0.999f ? float3(0.0f, 0.0f, 1.0f) : float3(1.0f, 0.0f, 0.0f);
  float3 TangentX = normalize(cross(UpVector, N));
  float3 TangentY = cross(N, TangentX);

  return TangentX * H.x + TangentY * H.y + N * H.z;
}

//------------------------------------------------------------------------------------//
// Used by IblBrdf.hlsl generation and IblImportanceSamplingSpecular.fx               //
// Inputs:                                                                            //
//   Spherical hammersley generated coordinate and roughness.                         //
//   Roughness                                                                        //
//   Normal                                                                           //
// Base on GGX example in:                                                            //
// http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
//------------------------------------------------------------------------------------//
float3 importanceSampleGGX(float2 Xi, float roughness, float3 N)
{
  float a = roughness * roughness;

  float Phi = 2 * PI * Xi.x;
  float CosTheta = sqrt((1 - Xi.y) / (1 + (a * a - 1) * Xi.y));
  float SinTheta = sqrt(1 - CosTheta * CosTheta);

  float3 H;
  H.x = SinTheta * cos(Phi);
  H.y = SinTheta * sin(Phi);
  H.z = CosTheta;

  float3 UpVector = abs(N.z) < 0.999f ? float3(0, 0, 1) : float3(1, 0, 0);
  float3 TangentX = normalize(cross(UpVector, N));
  float3 TangentY = cross(N, TangentX);

  return TangentX * H.x + TangentY * H.y + N * H.z;
}

//------------------------------------------------------------------------------------//
// Shader functions used by IblImportanceSamplingSpecular.fx                          //
//------------------------------------------------------------------------------------//
// D(h) for GGX.
// http://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
float specularD(float roughness, float NoH)
{
  float r2 = roughness * roughness;
  float NoH2 = NoH * NoH;
  float a = 1.0f / (3.14159f * r2 * pow(NoH, 4.0f));
  float b = exp((NoH2 - 1.0f) / r2 * NoH2);
  return a * b;
}

float4
sumSpecular(float3 hdrPixel, float NoL, float4 result)
{
  result.xyz += (hdrPixel * NoL);
  result.w += NoL;
  return result;
}

// Sum the diffuse term while iterating over all samples.
float4
sumDiffuse(float3 diffuseSample, float NoV, float4 result)
{
  result.xyz += diffuseSample;
  result.w++;
  return result;
}

float
t2p(float t, int noOfPixels)
{
  return (t * float(noOfPixels - 0.5f));
}

float3
sphericalEnvMapToDirection(float2 uv)
{
  // Convert spherical coordinates to Cartesian coordinates
  float theta = PI * (1.0f - uv.y); // azimuthal angle
  float phi = 2.0f * PI * (0.5f - uv.x); // polar angle
  return float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
}

float2
directionToSphericalEnvMap(float3 dir)
{
  float phi = -atan2(dir.y, dir.x); // azimuthal angle
  float theta = acos(dir.z);
  float s = 0.5f - phi * RECIPROCAL_2PI;
  float t = 1.0f - theta * RECIPROCAL_PI;
  return float2(s, t);
}

float3
random_pcg3d(uint3 v)
{
  v = v * 1664525u + 1013904223u;
  v.x += v.y * v.z;
  v.y += v.z * v.x;
  v.z += v.x * v.y;
  v ^= v >> 16u;
  v.x += v.y * v.z;
  v.y += v.z * v.x;
  v.z += v.x * v.y;
  return float3(v) / float(0xffffffffu);
}

float3x3
getNormalFrame(float3 normal)
{
  float3 someVec = float3(1.0f, 0.0f, 0.0f);
  float dd = dot(someVec, normal);
  float3 tangent = (1.0f - abs(dd) > 1e-6f) ?
                   normalize(cross(someVec, normal)) :
                   float3(0.0f, 1.0f, 0.0f);
  float3 bitangent = cross(normal, tangent);
  return float3x3(tangent, bitangent, normal);
}
