cbuffer CommonValues : register(b0)
{
  float4x4 matView;
  float4x4 matViewTranspose;
  float4x4 matViewInverse;
  float4x4 matViewInverseTranspose;

  float4x4 matProjection;
  float4x4 matProjectionTranspose;
  float4x4 matProjectionInverse;
  float4x4 matProjectionInverseTranspose;
    
  float2 viewPort;
  float nearClip;
  float farClip;
    
  float4 viewPos;
  float4 viewTarget;
}
