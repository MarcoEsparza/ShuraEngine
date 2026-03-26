#version 430 core

#include "ShaderConstants.glsl"

in vec2 TexCoords;
out vec4 FragColor;

layout (binding = 0) uniform sampler2D t_posMap;
layout (binding = 1) uniform sampler2D t_normalMap;

float saturate(float x)
{
  return clamp(x, 0.0, 1.0);
}

vec3 getPosition(vec2 uv)
{
  return texture(t_posMap, uv).xyz;
}

vec4 getNormal(vec2 uv)
{
  return texture(t_normalMap, uv);
}

vec3 getRandom(vec2 uv)
{
  float noiseX = fract(sin(dot(uv, vec2(15.8989, 76.132))) * 46336.23745);
  float noiseY = fract(sin(dot(uv, vec2(11.9899, 62.223))) * 34748.34744);
  float noiseZ = fract(sin(dot(uv, vec2(13.3238, 63.122))) * 59998.47362);

  return normalize(vec3(noiseX, noiseY, noiseZ) * 2.0 - 1.0);
}

float computeAO(vec2 tcoord, vec2 uv, vec3 p, vec3 cnorm)
{
  vec3 diff = getPosition(tcoord + uv) - p;
  float d = length(diff) * aoScale;
  vec3 v = normalize(diff);

  return max(0.0, dot(cnorm, v) - aoBias) * (1.0 / (1.0 + d)) * aoIntensity;
}

void main()
{
  vec2 screenUV = TexCoords;
  vec4 normal = getNormal(screenUV);

  if (normal.w == 0.0)
  {
    discard;
  }

  vec3 pos = getPosition(screenUV);
  vec3 n = normal.xyz;
  vec3 rand = getRandom(screenUV);

  float ao = 0.0;
  float rad = sampleRad / -pos.x;

  vec2 vecs[4] = vec2[](
    vec2( 1.0,  0.0),
    vec2(-1.0,  0.0),
    vec2( 0.0,  1.0),
    vec2( 0.0, -1.0)
  );

  int iter = 4;

  for (int j = 0; j < iter; ++j)
  {
    vec2 coord1 = reflect(vecs[j], rand.xy) * rad;

    vec2 coord2 = vec2(
      coord1.x * 0.707 - coord1.y * 0.707,
      coord1.x * 0.707 + coord1.y * 0.707
    );

    ao += computeAO(screenUV, coord1 * 0.25, pos, n);
    ao += computeAO(screenUV, coord2 * 0.5,  pos, n);
    ao += computeAO(screenUV, coord2 * 0.75, pos, n);
    ao += computeAO(screenUV, coord2,        pos, n);
  }

  ao /= (iter * 4);

  FragColor = vec4(vec3(1.0 - ao), 1.0);
}
