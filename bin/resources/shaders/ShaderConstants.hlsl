SamplerState samplerLinearWrap : register(s0);
SamplerState samplerPointWrap : register(s1);
SamplerState samplerAnisotropicWrap : register(s2);
SamplerState samplerLinearClamp : register(s3);
SamplerState samplerPointClamp : register(s4);
SamplerState samplerAnisotropicClamp : register(s5);

#ifndef PI
#define PI 3.14159265359
#endif
#ifndef RECIPROCAL_PI
#define RECIPROCAL_PI 1.0f / 3.14159265359
#endif
#ifndef RECIPROCAL_2PI
#define RECIPROCAL_2PI 1.0f / (2 * 3.14159265359)
#endif

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
  
  
  float lightIntensity; // Global light intensity
  float middleGrey; // Middle grey value for tone mapping
  
  float cubeRoughness; // Roughness value for environment mapping
  uint cubeFaceIndex; // Face index for environment mapping
  float2 sdPadding; // Shader data padding
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

// ----------------------------------------------------------------------------

float2
getSkyBoxUV(float3 dir)
{
  float u = -atan2(dir.z, dir.x) * RECIPROCAL_2PI + 0.5f;
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
  
  //return float(bits) * (1.0f / float(4294967296.0));
  return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

// ----------------------------------------------------------------------------

float2
hammersley(uint i, uint N)
{
  return float2(float(i) / float(N), radicalInverse_VdC(i));
}

// ----------------------------------------------------------------------------

float3
importanceSampleGGX(float2 Xi, float3 N, float roughness)
{
  float a = roughness * roughness;
  float phi = 2.0f * PI * Xi.x;
  float cosTheta = sqrt((1.0f - Xi.y) / (1.0f + (a * a - 1.0f) * Xi.y));
  float sinTheta = sqrt(1.0f - cosTheta * cosTheta);
  
  // from spherical coordinates to cartesian coordinates
  float3 H;
  H.x = cos(phi) * sinTheta;
  H.y = sin(phi) * sinTheta;
  H.z = cosTheta;
  
  // from tangent-space vector to world-space sample vector
  float3 up = abs(N.z) < 0.999 ? float3(0.0, 0.0, 1.0) : float3(1.0, 0.0, 0.0);
  float3 tangent = normalize(cross(up, N));
  float3 bitangent = cross(N, tangent);
  
  float3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
  return normalize(sampleVec);
}
