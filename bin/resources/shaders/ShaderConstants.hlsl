SamplerState samplerLinearWrap : register(s0);
SamplerState samplerPointWrap : register(s1);
SamplerState samplerAnisotropicWrap : register(s2);
SamplerState samplerLinearClamp : register(s3);
SamplerState samplerPointClamp : register(s4);
SamplerState samplerAnisotropicClamp : register(s5);

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
};

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
    
  return props;
}

float4
sRGBToLinear(float4 color)
{
  // Convert sRGB to linear color space
  return float4(pow(color.rgb, 1.0f / 2.2f), color.a);
}
