#version 430 core

#include "ShaderConstants.glsl"

#define DELTA 1e-8

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(binding = 0) uniform sampler2D t_inputMap;
layout(binding = 1) uniform sampler2D t_texture1; // bloom
layout(binding = 2) uniform sampler2D t_texture2; // luminance mip chain
layout(binding = 3) uniform sampler3D lutTex;

layout(binding = 0, rgba32f) uniform image2D t_outputMap;

vec3 reinhard(vec3 c)
{
  return (c * (1.0 + c / (whitePoint * whitePoint))) / (c + 1.0);
}

vec3 aces(vec3 c)
{
  const float a = 2.51;
  const float b = 0.03;
  const float c1 = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp((c * (a * c + b)) / (c * (c1 * c + d) + e), 0.0, 1.0);
}

vec3 uncharted2(vec3 x)
{
  float A=0.15, B=0.50, C=0.10, D=0.20, E=0.02, F=0.30;
  vec3 mapped = ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
  return mapped * whitePoint;
}

vec3 agx(vec3 x)
{
  x = max(x, 0.0);
  const float a=2.521, b=0.03, c=2.437, d=0.4, e=0.03;
  return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}

vec3 lutToneMap(vec3 c)
{
  return textureLod(lutTex, c, 0.0).rgb;
}

vec3 getTone(vec3 c)
{
  if (toneMapIndex == 0.0) return reinhard(c);
  if (toneMapIndex == 1.0) return aces(c);
  if (toneMapIndex == 2.0) return uncharted2(c);
  if (toneMapIndex == 3.0) return agx(c);
  if (toneMapIndex == 4.0) return lutToneMap(c);
  return c;
}

void main()
{
  uvec2 id = gl_GlobalInvocationID.xy;

  if (id.x >= uint(screenSize.x) || id.y >= uint(screenSize.y)) {
    return;
  }

  ivec2 coord = ivec2(id);
  vec3 color = texelFetch(t_inputMap, coord, 0).rgb;
  vec2 uv = (vec2(id) + 0.5) / vec2(screenSize);
  vec3 bloom = textureLod(t_texture1, uv, 0.0).rgb;

  float mip = float(textureQueryLevels(t_texture2) - 1);
  float avgLogLum = textureLod(t_texture2, vec2(0.5), mip).r;
  float avgLum = exp(avgLogLum);

  vec3 exposed = color * middleGrey / (avgLum + DELTA);
  vec3 mapped = getTone(exposed);

  mapped += bloom * bloomMultiplier;
  mapped = pow(mapped, vec3(1.0 / 2.2));

  imageStore(t_outputMap, coord, vec4(clamp(mapped, 0.0, 1.0), 1.0));
}
