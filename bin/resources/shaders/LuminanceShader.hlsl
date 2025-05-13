SamplerState textureSampler : register(s0);
Texture2D<float4> t_inputMap : register(t0);
RWTexture2D<float4> t_outputMap : register(u0);

[numthreads(16, 16, 1)]
void
CSMain( uint3 dtID : SV_DispatchThreadID )
{

}