SamplerState textureSampler : register(s0);
Texture2D t_DepthMap : register(t0);
Texture2D t_NormalMap : register(t1);
Texture2D t_ColorMap : register(t2);
Texture2D t_AOMap : register(t3);

cbuffer InvVP : register(b0)
{
  float4x4 InvViewProj;
  float4x4 InvView;
}

cbuffer ViewDir : register(b1)
{
  float4 ViewPos;
}

cbuffer Light : register(b2)
{
  float4 LightPos[12];
}

cbuffer Viewport : register(b3)
{
  float2 Dimensions;
  float FarClip;
  float NearClip;
}

struct PS_INPUT
{
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

PS_INPUT main(uint vertexID : SV_VertexID)
{
  PS_INPUT output = (PS_INPUT) 0;
  
  float2 position[3] =
  {
    float2(-1.0f, -1.0f),
    float2(3.0f, -1.0f),
    float2(-1.0f, 3.0f)
  };
  
  output.Position = float4(position[vertexID], 0.0f, 1.0f);
  output.Texcoord = (position[vertexID] + float2(1.0f, 1.0f)) * 0.5f;
  output.Texcoord.y = 1.0f - output.Texcoord.y;
    
  return output;
}

float4 mainPS(PS_INPUT input) : SV_Target
{
  float2 screenUV = input.Position.xy / Dimensions;
  
  float4 depth = t_DepthMap.Sample(textureSampler, screenUV);
  float4 normal = t_NormalMap.Sample(textureSampler, screenUV);
  float4 color = t_ColorMap.Sample(textureSampler, screenUV);
  float4 ao = t_AOMap.Sample(textureSampler, screenUV);
    
  if (normal.w == 0)
  {
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
  }
  
  normal.xyz = normal.xyz * 2.0f - 1.0f;
  normal.xyz = mul(float4(normal.xyz, 0.0f), InvView);
    
  float2 xyClipPos = screenUV * 2.0f - 1.0f;
  float4 positionClip = float4(xyClipPos, depth.x * (FarClip - NearClip), 1.0f);
  float4 posWorld = float4(depth.xyz, 1.0f);
    
  float3 lightDir = normalize(LightPos[0].xyz - posWorld.xyz);
  //float3 viewDirection = normalize(ViewPos.xyz - posWorld.xyz);
  float NdL = saturate(dot(normal.xyz, lightDir));
    
  float3 ambientLight = 0.03f * color.rgb;
    
  float3 finalColor = pow((color.rgb * NdL + ambientLight) * ao.r, 1.0f / 2.2f);
  return float4(finalColor, 1.0f);
}