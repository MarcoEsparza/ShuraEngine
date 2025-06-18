
cbuffer MainBuffer : register(b0)
{
  float4x4 matView;
  float4x4 matViewTranspose;
  float4x4 matViewInverse;
  float4x4 matViewInverseTranspose;

  float4x4 matProjection;
  float4x4 matProjectionTranspose;
  float4x4 matProjectionInverse;
  float4x4 matProjectionInverseTranspose;
    
  float2 screenSize;
  float nearClip;
  float farClip;
    
  float4 viewPos;
  float4 viewTarget;
    
  float time;
  float deltaTime;
  float cosTime;
  float sinTime;
}

cbuffer ShaderData : register(b1)
{
  // Ambient occlusion settings
  float randomSize;
  float sampleRad;
  float aoScale;
  float aoBias;
  float aoIntensity;
    
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
