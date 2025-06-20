
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
  float exposure;
  float brightThreshold;
    
  // Post-processing settings
  float minR;
  float maxR;
  float minG;
  float maxG;
  float minB;
  float maxB;
}
