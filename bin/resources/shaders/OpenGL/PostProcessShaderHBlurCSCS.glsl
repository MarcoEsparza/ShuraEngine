#version 430 core

#include "ShaderConstants.glsl"

#define BLURH_THREADS_X 4
#define BLURH_THREADS_Y 64
#define BLURV_THREADS_X 64
#define BLURV_THREADS_Y 4
#define BLUR_WIDTH 9
#define HALF_BLUR_WIDTH 4

layout(local_size_x = 1, local_size_y = BLURH_THREADS_Y, local_size_z = 1) in;

layout(binding = 0) uniform sampler2D t_inputMap;
layout(binding = 0, rgba32f) uniform image2D t_outputMap;

const float weights[9] = float[](
  0.004815026,0.028716039,0.102818575,
  0.221024189,0.28525234,0.221024189,
  0.102818575,0.028716039,0.004815026
);

void main()
{
  ivec2 base = ivec2(gl_GlobalInvocationID.x * BLURH_THREADS_X, gl_GlobalInvocationID.y);
  vec4 input[BLUR_WIDTH + BLURH_THREADS_X];
  int mip = int(mipLevel0);

  for(int i = 0; i < BLUR_WIDTH + BLURH_THREADS_X; i++) {
    input[i] = texelFetch(t_inputMap, base + ivec2(i, 0), mip);
  }

  for(int i = 0; i < BLURH_THREADS_X; i++) {
    vec4 sum = vec4(0.0);
    for(int j = 0; j < BLUR_WIDTH; j++) {
    sum += input[i + j] * weights[j];
    }
    imageStore(t_outputMap, base + ivec2(i, 0), sum);
  }
}
