/*****************************************************************************/
/*
*  @file    shShaderManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/09/30
*  @brief   Shader managment module.
*
*  Shader managment module.
*
*  @bug     No bug known.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesCore.h"
#include "shModule.h"
#include "shMaterial.h"
#include <shVector2.h>
#include <shVector3.h>
#include <shVector4.h>
#include <shMatrix4.h>
#include <shLinearColor.h>

#define DEFAULT_SHADOW_MAP_SIZE                               2048.0f

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Foward declarations
*/
/*****************************************************************************/
class Pass;
class ConstantBuffer;
//struct MaterialProperties;

/**
*  @brief Main buffer data structure.
*/
struct SH_CORE_EXPORT MainBufferData
{
  // View matrix data from the camera.
  Matrix4 viewMatrix = Matrix4::IDENTITY;
  Matrix4 transposeViewMatrix = Matrix4::IDENTITY;
  Matrix4 inverseViewMatrix = Matrix4::IDENTITY;
  Matrix4 inverseTransposeViewMatrix = Matrix4::IDENTITY;

  // Projection matrix data from the camera.
  Matrix4 projectionMatrix = Matrix4::IDENTITY;
  Matrix4 transposeProjectionMatrix = Matrix4::IDENTITY;
  Matrix4 inverseProjectionMatrix = Matrix4::IDENTITY;
  Matrix4 inverseTransposeProjectionMatrix = Matrix4::IDENTITY;

  Matrix4 inverseViewProjMatrix = Matrix4::IDENTITY;
  Matrix4 inverseTransposeViewProjMatrix = Matrix4::IDENTITY;

  // Viewport dimensions.
  Vector2 screenSize = { 0.0f, 0.0f };
  float nearPlane = 0.0f;
  float farPlane = 0.0f;

  // Camera settings.
  Vector4 cameraPosition = { 0.0f, 0.0f, 0.0f, 0.0f };
  Vector4 cameraDirection = { 0.0f, 0.0f, 0.0f, 0.0f };

  // Time data.
  float time = 0.0f;
  float deltaTime = 0.0f;
  float cosTime = 0.0f;
  float sinTime = 0.0f;
};

/**
*  @brief Shader data structure.
*/
struct SH_CORE_EXPORT ShaderData {
  // Ambient occlusion data.
  float sampleRadius = 1.0f;
  float aoScale = 1.0f;
  float aoBias = 0.01f;
  float aoIntensity = 1.0f;

  // ShadowMap
  float shadowMapSize = DEFAULT_SHADOW_MAP_SIZE;

  // Tone mapping data.
  float toneMappingIndex = 0.0f;
  float lutSize = 0.0f;
  float whitePoint = 1.0f;
  float bloomMultiplier = 1.0f;
  float brightThreshold = 1.0f;

  // Post-processing data.
  float minR = 0.0f;
  float maxR = 1.0f;
  float minG = 0.0f;
  float maxG = 1.0f;
  float minB = 0.0f;
  float maxB = 1.0f;

  // Mip levels for texture sampling.
  float mipLevel0 = 0.0f;
  float mipLevel1 = 0.0f;

  float lightIntensity = 1.0f;
  float middleGrey = 1.0f;

  float roughness = 0.0f;
  uint32 cubeFace = 0;
  Vector2 padding = { 0.0f, 0.0f };
};

struct SH_CORE_EXPORT PrefilteredCB
{
  uint32 width = 0;
  uint32 height = 0;
  uint32 samples = 0;
  float roughness = 0.0f;
  float mipmapLevels = 0.0f;
  Vector3 padding = Vector3::ZERO;

  PrefilteredCB() = default;
  PrefilteredCB(uint32 w, uint32 h, uint32 s, float r, float m)
    : width(w), height(h), samples(s), roughness(r), mipmapLevels(m)
  {}
};

/**
*  @brief Light data structure for constant buffer.
*/
struct SH_CORE_EXPORT LightCB
{
  Vector4 position = { 0.0f, 0.0f, 0.0f, 1.0f };
  Vector3 target = Vector3::ZERO;
  float intensity = 1.0f;
  LinearColor color = LinearColor::WHITE;
  Matrix4 view = Matrix4::IDENTITY;
  Matrix4 proj = Matrix4::IDENTITY;
};

struct PBRMaterialData {
  Vector3 baseColorFactor;
  float unused0; // Padding to align to 16 bytes
  Vector2 metallicRoughnessFactor;
  MaterialProperties properties;
  float unused1; // Padding to align to 16 bytes
};

/**
*  @brief Shader managment module.
*/
class SH_CORE_EXPORT ShaderManager : public Module<ShaderManager>
{
 public:
  /**
  *  @brief Default constructor.
  */
  ShaderManager() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~ShaderManager() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  void
  createPipelinePasses();

  SPtr<Pass>
  getPassFromMaterial(const MaterialProperties& props);

  void
  recompileShaders();

  void
  updateMainCB();

  void
  updateShaderDataCB();

  void
  updatePrefilterShaderCB();

  void
  updateLightCB();

  void
  updateMaterialCB();

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  UMap<uint32, SPtr<Pass>> m_passes;

  /**
  *  @brief Main Constant Buffer.
  */
  SPtr<ConstantBuffer> m_pMainBuffer;

  /**
  *  @brief Shader Data Constant Buffer.
  */
  SPtr<ConstantBuffer> m_pShaderDataBuffer;

  SPtr<ConstantBuffer> m_pPrefilteredCB;

  /**
  *  @brief Constant Buffer for light struct.
  */
  SPtr<ConstantBuffer> m_pLightBuffer;

  /**
  *  @brief Constant buffer for light camera.
  */
  SPtr<ConstantBuffer> m_pLCBuffer;

  /**
  *  @brief Model transform.
  */
  SPtr<ConstantBuffer> m_pModelTransformBuffer;

  SPtr<ConstantBuffer> m_pPBRData;

  MainBufferData m_mainBufferData;

  ShaderData m_shaderData;

  PrefilteredCB m_prefilteredData;

  LightCB m_lightData;

  PBRMaterialData m_materialData;

  static const uint32 SSAO_SHADER_ID;
  static const uint32 HBLUR_SHADER_ID;
  static const uint32 VBLUR_SHADER_ID;
  static const uint32 HBLUR_CS_ID;
  static const uint32 VBLUR_CS_ID;
  static const uint32 LIGHT_CS_ID;
  static const uint32 SHADOWMAP_SHADER_ID;
  static const uint32 SKYBOX_SHADER_ID;
  static const uint32 FINAL_SHADER_ID;
  static const uint32 PLANE_SHADER_ID;
  static const uint32 HISTOGRAM_SHADER_ID;
  static const uint32 ADDSKYBOX_SHADER_ID;
  static const uint32 LUMINANCE_SHADER_ID;
  static const uint32 BRIGHT_SHADER_ID;
  static const uint32 TONEMAP_SHADER_ID;
  static const uint32 POSTPROCESS_SHADER_ID;
  static const uint32 ADDITIVE_SHADER_ID;
  static const uint32 CUBEMAP_SHADER_ID;
  static const uint32 DIFFUSE_IRR_SHADER_ID;
  static const uint32 PREFILTERED_IRR_SHADER_ID;
  static const uint32 BRDF_SHADER_ID;
};

/**
*  @brief Easier way to access the ShaderManager module.
*/
SH_CORE_EXPORT ShaderManager&
g_shaderMan();
}
