/*****************************************************************************/
/*
*  @file    shRenderManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/23
*  @brief   Render module.
*
*  Render module.
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
#include "shVector2.h"
#include "shLinearColor.h"
#include "shGraphicTypes.h"
#include "shMatrix4.h"

#define MAX_CONSTANT_BUFFER_SLOTS                             14
#define MAX_SHADER_RESOURCE_VIEW_SLOTS                        128
#define DEFAULT_SHADOW_MAP_SIZE                               2048.0f

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Foward declarations
*/
/*****************************************************************************/
class Material;
class Pass;
class BlendState;
class Texture2D;
class ConstantBuffer;
class SceneGraph;
class SamplerState;

/**
*  @brief Render target information structure.
*/
struct RenderTargetInfo
{
  RenderTargetInfo() = default;
  RenderTargetInfo(const String& name,
    uint32 format = TEXTURE_FORMAT::kR8G8B8A8_UNORM,
    uint32 bFlags = BIND_FLAGS::kRenderTarget | BIND_FLAGS::kShaderResource,
    uint32 usage = USAGE::kDefault,
    uint32 mipLevels = 1,
    float width = 1.0f,
    float height = 1.0f,
    bool bUseScaledSize = true)
    : name(name), format(format), usage(usage), bFlags(bFlags), mipLevels(mipLevels),
    width(width), height(height), bUseScaledSize(bUseScaledSize)
  {}

  String name;
  uint32 format = 0;
  uint32 usage = 0;
  uint32 bFlags = 0;
  uint32 mipLevels = 1;
  float width = 1.0f;
  float height = 1.0f;
  bool bUseScaledSize = true;
  SPtr<Texture2D> pTexture;
};

/**
*  @brief Main buffer data structure.
*/
struct MainBufferData
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
struct ShaderData {
  // Ambient occlusion data.
  float sampleRadius = 0.0f;
  float aoScale = 0.0f;
  float aoBias = 0.0f;
  float aoIntensity = 0.0f;

  // ShadowMap
  float shadowMapSize = 0.0f;

  // Tone mapping data.
  float toneMappingIndex = 0.0f;
  float lutSize = 0.0f;
  float whitePoint = 0.0f;
  float bloomMultiplier = 0.0f;
  float brightThreshold = 0.0f;

  // Post-processing data.
  float minR = 0.0f;
  float maxR = 0.0f;
  float minG = 0.0f;
  float maxG = 0.0f;
  float minB = 0.0f;
  float maxB = 0.0f;

  // Mip levels for texture sampling.
  float mipLevel0 = 0.0f;
  float mipLevel1 = 0.0f;
  
  float lightIntensity = 0.0f;
  float middleGrey = 0.0f;
};

/**
*  @brief Render module.
*/
class SH_CORE_EXPORT RenderManager : public Module<RenderManager>
{
 public:
  /**
  *  @brief Default constructor.
  */
  RenderManager() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~RenderManager();

  /**
  *  @brief Override event for module.
  */
  void
  onStartUp() override;

  /**
  *  @brief Override event for module.
  */
  void
  onShutDown() override;

  /**
  *  @brief Creates all textures for the render pipeline.
  */
  void
  createRenderTextures();

  /**
  *  @brief Creates all passes for the render pipeline.
  */
  void
  createPasses();

  /**
  *  @brief Returns a pass with a given name.
  * 
  *  @param String& passName
  * 
  *  @return SPtr<Pass>
  */
  SPtr<Pass>
  getPass(const String& passName);

  SPtr<Texture2D>
  getTexture(const String& texName);

  /**
  *  @brief Recompile the shaders on the storaged passes.
  */
  void
  recompileShaders();

  /**
  *  @brief Draw all static meshes on scene
  */
  void
  drawStaticMeshOnScene();

  /**
  *  @brief Sets the resource view from PBRMaterial.
  * 
  *  @param SPtr<PBRMaterial>& pMat
  */
  void
  setResourceViewFromPBRMaterial(const SPtr<Material>& pMat);

  /**
  *  @brief Render the scene.
  */
  void
  renderScene();

  /**
  *  @brief Clean the PS shader resource view slots.
  *
  *  @param uint32 numSRV = MAX_SHADER_RESOURCE_VIEW_SLOTS
  */
  void
  cleanPSShaderResourceView(uint32 numSRV = MAX_SHADER_RESOURCE_VIEW_SLOTS);

  /**
  *  @brief Clean the CS shader resource view slots.
  *
  *  @param uint32 numSRV = MAX_SHADER_RESOURCE_VIEW_SLOTS
  */
  void
  cleanCSShaderResourceView(uint32 numSRV = MAX_SHADER_RESOURCE_VIEW_SLOTS);

  /**
  *  @brief Clean the CS unordered access view slots.
  *
  *  @param uint32 numSRV = MAX_SHADER_RESOURCE_VIEW_SLOTS
  */
  void
  cleanCSUAView(uint32 numSRV = MAX_SHADER_RESOURCE_VIEW_SLOTS);

  /**
  *  @brief Clean the vertex shader constant buffer slots.
  *
  *  @param uint32 numCB = MAX_CONSTANT_BUFFER_SLOTS
  */
  void
  cleanVSConstantBuffers(uint32 numCB = MAX_CONSTANT_BUFFER_SLOTS);

  /**
  *  @brief Clean the pixel shader constant buffer slots.
  *
  *  @param uint32 numCB = MAX_CONSTANT_BUFFER_SLOTS
  */
  void
  cleanPSConstantBuffers(uint32 numCB = MAX_CONSTANT_BUFFER_SLOTS);

  /**
  *  @brief Clean the geometry shader constant buffer slots.
  *
  *  @param uint32 numCB = MAX_CONSTANT_BUFFER_SLOTS
  */
  void
  cleanGSConstantBuffers(uint32 numCB = MAX_CONSTANT_BUFFER_SLOTS);

  /**
  *  @brief Clean the compute shader constant buffer slots.
  *
  *  @param uint32 numCB = MAX_CONSTANT_BUFFER_SLOTS
  */
  void
  cleanCSConstantBuffers(uint32 numCB = MAX_CONSTANT_BUFFER_SLOTS);

  /**
  *  @brief Call the other clean functions.
  */
  void
  cleanShaderObjects();

  /**
  *  @brief Set the shadow map texture size.
  * 
  *  @param float size
  */
  void
  setShadowMapSize(const float size);

  /**
  *  @brief Set the screen dimension info for the render manager.
  *
  *  @param Vector2& screenSize
  */
  void
  setScreenSize(const Vector2& screenSize);

  FORCEINLINE MainBufferData&
  getMainBufferData();

  FORCEINLINE ShaderData&
  getShaderData();

  FORCEINLINE SPtr<ConstantBuffer>&
  getMainBuffer();

  FORCEINLINE SPtr<ConstantBuffer>&
  getShaderDataBuffer();

 private:
  /**
  *  @brief Map to save passes.
  */
  UMap<uint32, SPtr<Pass>> m_passes;

  /**
  *  @brief Map to save targets.
  */
  UMap<uint32, RenderTargetInfo> m_renderTargetMap;

  /**
  *  @brief Blend state for basic geometry.
  */
  SPtr<BlendState> m_pBasicBS;

  /**
  *  @brief Blend state for alpha testing.
  */
  SPtr<BlendState> m_pAlphaTestBS;

  /**
  *  @brief Model transform.
  */
  SPtr<ConstantBuffer> m_pModelTransform;

  SPtr<ConstantBuffer> m_pPBRData;

  /**
  *  @brief Main Constant Buffer.
  */
  SPtr<ConstantBuffer> m_pMainBuffer;

  /**
  *  @brief Shader Data Constant Buffer.
  */
  SPtr<ConstantBuffer> m_pShaderDataBuffer;

  MainBufferData m_mainBufferData;

  ShaderData m_shaderData;

  SPtr<SamplerState> m_pSamplerClamp;

  /**
  *  @brief Shadow map texture size.
  */
  float m_shadowMapSize = DEFAULT_SHADOW_MAP_SIZE;

  /**
  *  @brief Screen size.
  */
  Vector2 m_screenDimension = { 0.0f, 0.0f };
};

FORCEINLINE MainBufferData&
RenderManager::getMainBufferData()
{
  return m_mainBufferData;
}

FORCEINLINE ShaderData&
RenderManager::getShaderData()
{
  return m_shaderData;
}

FORCEINLINE SPtr<ConstantBuffer>&
RenderManager::getMainBuffer()
{
  return m_pMainBuffer;
}

FORCEINLINE SPtr<ConstantBuffer>&
RenderManager::getShaderDataBuffer()
{
  return m_pShaderDataBuffer;
}

/**
*  @brief Easier way to access the RendererManager module.
*/
SH_CORE_EXPORT RenderManager&
g_renderMan();
}
