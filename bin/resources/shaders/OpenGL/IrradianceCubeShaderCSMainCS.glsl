#version 430 core

#include "ShaderConstants.glsl"

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

layout(binding = 0) uniform sampler2D t_skybox;

layout(binding = 0, rgba32f) uniform image2D t_output;

layout(std140, binding = 2) uniform PrefilterConstants
{
    uint width;
    uint height;
    uint samples;
    float roughness;
    float mipmapLevels;
    vec3 pcPadding;
};

float saturate(float x) { return clamp(x, 0.0, 1.0); }

void main()
{
  uvec2 dtID = gl_GlobalInvocationID.xy;

  if (dtID.x >= width || dtID.y >= height) {
    return;
  }

  vec2 texCoord = (vec2(dtID) + 0.5) / vec2(width, height);

  vec3 normal = sphericalEnvMapToDirection(texCoord);
  mat3 tbn = getNormalFrame(normal);

  vec3 result = vec3(0.0);
  float totalWeight = 0.0;

  const float maxBrightness = 30.0;

  for (uint n = 0u; n < samples; ++n) {
    vec3 rnd = random_pcg3d(uvec3(dtID, n));

    float phi = 2.0 * PI * rnd.x;
    float cosTheta = sqrt(1.0 - rnd.y);
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    vec3 posLocal = vec3(
      cos(phi) * sinTheta,
      sin(phi) * sinTheta,
      cosTheta
    );

    vec3 L = tbn * posLocal;

    float ndotL = saturate(dot(normal, L));

    if (ndotL > 0.0) {
      vec2 uv = directionToSphericalEnvMap(L);
      vec3 radiance = textureLod(t_skybox, uv, mipmapLevels).rgb;
      float brightness = max(radiance.r, max(radiance.g, radiance.b));

      if (brightness > maxBrightness) {
        radiance *= maxBrightness / brightness;
      }

      result += radiance * ndotL;
      totalWeight += ndotL;
    }
  }

  if (totalWeight > 0.0) {
    result /= totalWeight;
  }

  result /= PI;
  result *= 11.0;

  imageStore(t_output, ivec2(dtID), vec4(result, 1.0));
}
