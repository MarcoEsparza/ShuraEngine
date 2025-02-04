SamplerState textureSampler : register(s0);
Texture2D t_baseColor : register(t0);
Texture2D t_normal : register(t1);
Texture2D t_metallic : register(t2);
Texture2D t_roughness : register(t3);
Texture2D t_ambientO : register(t4);

cbuffer VP : register(b0)
{
  float4x4 View;
  float4x4 Proj;
}

cbuffer Model : register(b1)
{
  float4x4 ModelTransform;
}

cbuffer ViewDir : register(b2)
{
  float4 ViewPos;
}

cbuffer Light : register(b3)
{
  float3 LightPosition;
  float4 LightColor;
  float Intensity;
}

struct VS_INPUT
{
  float3 Position : POSITION;
  float3 Normal : NORMAL0;
  float2 Tex : TEXCOORD0;
  float3 Tangent : TANGENT0;
  float3 Bitangent : BINORMAL0;
};

struct PS_INPUT
{
  float4 Position : SV_POSITION;
  float3 Normal : NORMAL0;
  float2 Tex : TEXCOORD0;
  float3 Tangent : TANGENT0;
  float3 Bitangent : BINORMAL0;
};

PS_INPUT main(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT) 0;
  
  float4x4 wvp = mul(ModelTransform, mul(View, Proj));

  output.Position = mul(float4(input.Position.xyz, 1.0f), wvp);
  output.Normal = input.Normal;
  output.Tex = input.Tex;
  output.Tangent = input.Tangent;
  output.Bitangent = input.Bitangent;
  
  return output;
}

float4 mainPS(PS_INPUT input) : SV_Target
{
  float4 baseColor = t_baseColor.Sample(textureSampler, input.Tex);
  float roughnessMapColor = t_roughness.Sample(textureSampler, input.Tex).g;
  float metallicMapColor = t_metallic.Sample(textureSampler, input.Tex).b;
  float4 normalMapColor = float4(t_normal.Sample(textureSampler, input.Tex).rg, 1.0f, 1.0f) * 2.0f - 1.0f;
    
  float3x3 TBN = float3x3(input.Tangent, input.Bitangent, input.Normal);
    
  //normalMapColor.xyz = normalize(mul(normalMapColor.xyz, TBN));
  //return normalMapColor;

  float3 normal = normalize(mul(normalMapColor.xyz, TBN));
    
  //float diffuse = max(normal, 0.0f);
    
  float lightDir = LightPosition - input.Position.xyz;
    
  //float diffuse = max(dot(normal, LightPosition), 0.0f);
  float diffuse = max(dot(normal, lightDir), 0.0f);
    
  float3 viewDir = ViewPos.xyz - input.Position.xyz;
  float3 halfWayDir = normalize(lightDir + viewDir);
    
  //float3 halfVector = normalize(LightPosition + viewDir);
  
  float3 specular = pow(max(dot(normal, halfWayDir), 0.0f), Intensity);
  specular *= 1.0f - roughnessMapColor;
  float3 diffuseColor = baseColor.rgb * (1.0f - metallicMapColor);
  float3 specularColor = lerp(float3(0.04f, 0.04f, 0.04f), baseColor.rgb, metallicMapColor);
    
  //float3 finalColor = baseColor.rgb * diffuse;
    
  float3 ambient = { 0.5f, 0.5f, 0.5f };
    
  //float3 finalColor = (ambient + diffuse * LightColor + specular) * baseColor.rgb;
  float3 finalColor = (ambient * diffuseColor) +
                      ((diffuse * LightColor.xyz) * diffuseColor) +
                      specular + LightColor.xyz + specularColor;
    
  //return t_baseColor.Sample(textureSampler, input.Tex);
    
  return float4(finalColor, baseColor.a);
}