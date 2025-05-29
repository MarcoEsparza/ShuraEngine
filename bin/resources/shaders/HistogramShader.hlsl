Texture2D t_inputMap : register(t0);
RWTexture2D<float4> t_outputMap : register(u0);

#ifndef NUM_BINS
#define NUM_BINS 256
#endif

cbuffer Viewport : register(b0)
{
  float2 Dimensions;
  float unused;
  float unused1;
}

[numthreads(32, 32, 1)]
void
CSMain(uint3 dtID : SV_DispatchThreadID)
{
  if (dtID.x >= Dimensions.x || dtID.y >= Dimensions.y) {
    return;
  }

  float4 color = t_inputMap.Load(int3(dtID.xy, 0));
  
  uint binR = clamp((uint)(color.r * (NUM_BINS - 1)), 0, NUM_BINS - 1);
  uint binG = clamp((uint)(color.g * (NUM_BINS - 1)), 0, NUM_BINS - 1);
  uint binB = clamp((uint)(color.b * (NUM_BINS - 1)), 0, NUM_BINS - 1);
    
  uint dummy;
  InterlockedAdd(t_outputMap[uint2(binR, 0)].x, asuint(1.0f), dummy);
  InterlockedAdd(t_outputMap[uint2(binG, 0)].y, asuint(1.0f), dummy);
  InterlockedAdd(t_outputMap[uint2(binB, 0)].z, asuint(1.0f), dummy);
}