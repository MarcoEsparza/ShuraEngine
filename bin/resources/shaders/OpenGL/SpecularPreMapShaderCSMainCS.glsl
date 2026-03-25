#version 430

#include "ShaderConstants.glsl"

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(binding = 0) uniform sampler2D t_envMap;

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
  uvec2 id = gl_GlobalInvocationID.xy;

  if (id.x >= width || id.y >= height) {
    return;
  }

  vec2 texCoord = (vec2(id) + 0.5) / vec2(width, height);

  vec3 normal = sphericalEnvMapToDirection(texCoord);
  mat3 tbn = getNormalFrame(normal);

  vec3 viewDir = normal;

  vec3 result = vec3(0.0);
  float totalWeight = 0.0;

  float alpha = roughness * roughness;

  float adaptFactor = saturate(roughness + mipmapLevels / 8.0);
  float maxBrightness = mix(20.0, 80.0, adaptFactor);

  for (uint n = 0u; n < samples; ++n) {
    vec3 rnd = random_pcg3d(uvec3(id, n));

    float phi = 2.0 * PI * rnd.x;
    float u = rnd.y;

    float theta = acos(sqrt((1.0 - u) / (1.0 + (alpha * alpha - 1.0) * u)));

    vec3 posLocal = vec3(
      sin(theta) * cos(phi),
      sin(theta) * sin(phi),
      cos(theta)
    );

    vec3 H = tbn * posLocal;
    vec3 L = normalize(2.0 * dot(viewDir, H) * H - viewDir);

    float nDotL = dot(normal, L);

    if (nDotL > 0.0) {
      vec2 uv = directionToSphericalEnvMap(L);

      vec3 radiance = textureLod(t_envMap, uv, mipmapLevels).rgb;

      float lum = dot(radiance, vec3(0.2126, 0.7152, 0.0722));
      float maxC = max(radiance.r, max(radiance.g, radiance.b));
      float brightness = max(lum, maxC * 0.5);

      if (brightness > maxBrightness) {
        radiance *= (maxBrightness / brightness);
      }

      result += radiance * nDotL;
      totalWeight += nDotL;
    }
  }

  if (totalWeight > 0.0) {
    result /= totalWeight;
  }

  // Energy compensation
  float energyComp = 1.0 / (1.0 + 0.5 * roughness);
  result *= energyComp;

  imageStore(t_output, ivec2(id), vec4(result, 1.0));
}
