#version 450 core

#include "ShaderConstants.glsl"

// ----------------------------------------------------------------------------
// INPUTS
// ----------------------------------------------------------------------------

in vec2 Tex;
in vec3 Normal;
in vec3 Tangent;
in vec3 Bitangent;
in vec3 Depth;

// ----------------------------------------------------------------------------
// OUTPUTS (GBUFFER)
// ----------------------------------------------------------------------------

layout(location = 0) out vec4 outDepth;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outColor;
layout(location = 3) out vec4 outProperties;
layout(location = 4) out vec4 outEmissive;

// ----------------------------------------------------------------------------
// TEXTURES
// ----------------------------------------------------------------------------

layout(binding = 0) uniform sampler2D t_baseColor;
layout(binding = 1) uniform sampler2D t_normal;
layout(binding = 2) uniform sampler2D t_metallic;
layout(binding = 3) uniform sampler2D t_roughness;
layout(binding = 4) uniform sampler2D t_ambientO;
layout(binding = 5) uniform sampler2D t_emissive;
layout(binding = 6) uniform sampler2D t_specular;
layout(binding = 7) uniform sampler2D t_opacityMask;

// ----------------------------------------------------------------------------
// MATERIAL DATA
// ----------------------------------------------------------------------------

layout(std140, binding = 3) uniform MaterialData
{
  vec3 baseColorFactor;
  float alphaCutoff;

  vec2 metallicRoughnessFactors;

  int materialBitfield;
  float unused1;

  vec3 emissiveFactor;
  float matEmmissiveIntensity;
};

// ----------------------------------------------------------------------------

#ifndef ALPHA_TEST_THRESHOLD
#define ALPHA_TEST_THRESHOLD 0.5
#endif

// ----------------------------------------------------------------------------

void main()
{
  MaterialProperties materialProps = getMaterialProperties(materialBitfield);

  // --------------------------------------------------------------------------
  // DIFFUSE
  // --------------------------------------------------------------------------
#ifdef HAS_DIFFUSE_MAP
  outColor = texture(t_baseColor, Tex);
  outColor *= vec4(baseColorFactor, 1.0);
#else
  outColor = vec4(baseColorFactor, 1.0);
#endif

  // --------------------------------------------------------------------------
  // OPACITY
  // --------------------------------------------------------------------------
#ifdef HAS_OPACITY_MASK
  float opacity = texture(t_opacityMask, Tex).r;
  outColor.a *= opacity;
#endif

  // --------------------------------------------------------------------------
  // ALPHA TEST
  // --------------------------------------------------------------------------
#ifdef HAS_ALPHA_TESTING
  if (outColor.a < ALPHA_TEST_THRESHOLD)
    discard;
#endif

  // --------------------------------------------------------------------------
  // NORMAL
  // --------------------------------------------------------------------------
#ifdef HAS_NORMAL_MAP
  vec3 n = texture(t_normal, Tex).xyz * 2.0 - 1.0;
#else
  vec3 n = vec3(1.0);
#endif

  mat3 TBN = mat3(Tangent, Bitangent, Normal);
  vec3 finalNormal = normalize(TBN * n);

  outNormal = vec4(finalNormal * 0.5 + 0.5, 1.0);

  // ----------------------------------------------------------------------------
  // DEPTH
  // ----------------------------------------------------------------------------
  outDepth = vec4(Depth, 1.0);

  // ----------------------------------------------------------------------------
  // METALLIC
  // ----------------------------------------------------------------------------
#ifdef HAS_METALNESS_MAP
  outProperties.b = texture(t_metallic, Tex).b;
#else
  outProperties.b = metallicRoughnessFactors.x;
#endif

  // ----------------------------------------------------------------------------
  // ROUGHNESS
  // ----------------------------------------------------------------------------
#ifdef HAS_ROUGHNESS_MAP
  outProperties.g = texture(t_roughness, Tex).g;
#else
  outProperties.g = metallicRoughnessFactors.y;
#endif

  // ----------------------------------------------------------------------------
  // AO
  // ----------------------------------------------------------------------------
#ifdef HAS_AO_MAP
  outProperties.r = texture(t_ambientO, Tex).r;
#else
  outProperties.r = 1.0;
#endif

  // ----------------------------------------------------------------------------
  // EMISSIVE
  // ----------------------------------------------------------------------------
#ifdef USE_EMISSION
  #ifdef HAS_EMISSIVE_MAP
    outEmissive = texture(t_emissive, Tex);
  #else
    outEmissive = vec4(emissiveFactor, 1.0);
  #endif
#else
  outEmissive = vec4(0.0);
#endif

  // ----------------------------------------------------------------------------
  // INVERT ROUGHNESS
  // ----------------------------------------------------------------------------
#ifdef INVERT_ROUGHNESS
  outProperties.b = 1.0 - outProperties.b;
#endif
}