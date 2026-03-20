#version 450 core

#include "ShaderConstants.glsl"

// ----------------------------------------------------------------------------
// INPUTS
// ----------------------------------------------------------------------------

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTex;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

// ----------------------------------------------------------------------------
// OUTPUTS
// ----------------------------------------------------------------------------

out vec2 Tex;
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;
out vec3 Depth;

// ----------------------------------------------------------------------------
// UNIFORMS
// ----------------------------------------------------------------------------

layout(std140, binding = 2) uniform Model
{
  mat4 ModelTransform;
};

// ----------------------------------------------------------------------------

void main()
{
  mat4 wvp = matProjection * matView * ModelTransform;

  gl_Position = wvp * vec4(inPosition, 1.0);

  Tex = inTex;

  vec4 worldPos = ModelTransform * vec4(inPosition, 1.0);
  Depth = worldPos.xyz;

  mat3 normalMat = mat3(ModelTransform);

  Normal = normalize(normalMat * inNormal);
  Tangent = normalize(normalMat * inTangent);
  Bitangent = normalize(normalMat * inBitangent);
}
