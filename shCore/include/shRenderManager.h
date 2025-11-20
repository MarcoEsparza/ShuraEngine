/*****************************************************************************/
/*
*  @file    shRenderManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/11/14
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
#include "shVector2i.h"

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
class Texture3D;
class ConstantBuffer;
class SceneGraph;
class SamplerState;
class CubeMap;
class GameObject;

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
    uint32 arraySize = 1,
    float width = 1.0f,
    float height = 1.0f,
    bool bUseScaledSize = true)
    : name(name), format(format), usage(usage), bFlags(bFlags), mipLevels(mipLevels),
    arraySize(arraySize), width(width), height(height), bUseScaledSize(bUseScaledSize)
  {}

  String name;
  uint32 format = 0;
  uint32 usage = 0;
  uint32 bFlags = 0;
  uint32 mipLevels = 1;
  uint32 arraySize = 1;
  float width = 1.0f;
  float height = 1.0f;
  bool bUseScaledSize = true;
  SPtr<Texture2D> pTexture;
};

struct SceneTarget
{
  SPtr<Texture2D> pTexture;
  uint32 width = 0;
  uint32 height = 0;
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
  virtual ~RenderManager() = default;

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

  SPtr<Texture2D>
  getTexture(const String& texName);

  /**
  *  @brief Draw all static meshes on scene
  */
  void
  drawMeshesOnScene();

  void
  drawStaticMesh(const WPtr<GameObject> pGO);

  void
  drawTransparentStaticMesh(const WPtr<GameObject> pGO);

  void
  drawSkeletalMesh(const WPtr<GameObject> pGO);

  void
  drawShadowMap();

  /**
  *  @brief Sets the resource view from PBRMaterial.
  * 
  *  @param SPtr<PBRMaterial>& pMat
  */
  void
  setShaderResourceFromMaterial(const SPtr<Material>& pMat);

  /**
  *  @brief Render the scene.
  */
  void
  renderScene();

  void
  computeIBL();

  void
  computeBRDF();

  SPtr<Texture2D>&
  createSceneTexture(const Vector2& winSize);

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

  void
  setSamplers();

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

 private:
  /**
  *  @brief Map to save targets.
  */
  UMap<uint32, RenderTargetInfo> m_renderTargetMap;

  SPtr<SamplerState> m_pSamplerLinearWrap;
  SPtr<SamplerState> m_pSamplerPointWrap;
  SPtr<SamplerState> m_pSamplerAnisotropicWrap;
  SPtr<SamplerState> m_pSamplerLinearClamp;
  SPtr<SamplerState> m_pSamplerPointClamp;
  SPtr<SamplerState> m_pSamplerAnisotropicClamp;

  SPtr<CubeMap> m_pLutTexture;
  SPtr<CubeMap> m_pLutLBK;

  SPtr<Texture2D> m_pBRDF;
  SPtr<Texture2D> m_pDiffIrr;
  SPtr<Texture2D> m_pSpecularPreMap;

  /**
  *  @brief Shadow map texture size.
  */
  float m_shadowMapSize = DEFAULT_SHADOW_MAP_SIZE;

  /**
  *  @brief Screen size.
  */
  Vector2 m_screenDimension = { 0.0f, 0.0f };

  Vector2i m_skyboxDimension = { 0, 0 };

  SceneTarget m_sceneTarget;

  static const uint32 SHADOWMAP_TEX_ID;
  static const uint32 SHADOWTEMP_TEX_ID;
  static const uint32 DEPTH_TEX_ID;
  static const uint32 NORMAL_TEX_ID;
  static const uint32 COLOR_TEX_ID;
  static const uint32 PROPS_TEX_ID;
  static const uint32 EMMISIVE_TEX_ID;
  static const uint32 SSAO_TEX_ID;
  static const uint32 HBLUR_TEX_ID;
  static const uint32 VBLUR_TEX_ID;
  static const uint32 SKYBOX_TEX_ID;
  static const uint32 LIGHTC_TEX_ID;
  static const uint32 TONEMAP_TEX_ID;
  static const uint32 TEMP_TEX_ID;
  static const uint32 LUMINANCE_TEX_ID;
  static const uint32 BRIGHT_TEX_ID;
  static const uint32 POSTPROCESS_TEX_ID;
  static const uint32 ADDITIVE_TEX_ID;
  static const uint32 BHBLUR_TEX_ID;
  static const uint32 BVBLUR_TEX_ID;
  static const uint32 GBUFFER_DEPTH_TEX_ID;
  static const uint32 HISTOGRAM_TEX_ID;
  static const uint32 EMM_PROCESS_TEX_ID;
  static const uint32 PLANE_DEPTH_TEX_ID;
};

/**
*  @brief Easier way to access the RendererManager module.
*/
SH_CORE_EXPORT RenderManager&
g_renderMan();
}
