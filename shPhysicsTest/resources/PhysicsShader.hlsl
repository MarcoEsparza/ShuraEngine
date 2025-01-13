SamplerState textureSampler : register(s0);
Texture2D textureData : register(t0);

cbuffer WVP : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Proj;
}

struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL0;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL0;
    float2 Tex : TEXCOORD0;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
  
    float4 worldPos = mul(float4(input.Position, 1.0), World);
    float4 viewPos = mul(worldPos, View);
    output.Position = mul(viewPos, Proj);
    
    //float4x4 wvp = mul(World, mul(View, Proj));

    //output.Position = mul(float4(input.Position.xyz, 1.0f), wvp);
    //output.Position = float4(input.Position.xyz, 1.0f);
    
    output.Normal = input.Normal;
    output.Tex = input.Tex;
  
    return output;
}

float4 mainPS(PS_INPUT input) : SV_Target
{
    //return textureData.Sample(textureSampler, input.Tex);
    
    float4 color = textureData.Sample(textureSampler, input.Tex);
    if (color.a < 0.5)
    {
        discard;
    }
    
    return color;
}