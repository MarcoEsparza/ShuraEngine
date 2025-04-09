/*****************************************************************************/
/*
*  @file    shRenderManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/08
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

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Foward declarations
*/
/*****************************************************************************/
class StaticMeshUnionComponent;
class Material;
class Pass;
class BlendState;
class Texture2D;
class ConstantBuffer;

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
  *  @brief Adds a render target to the render manager.
  * 
  *  @param SPtr<Texture2D>& pRTV
  *  @param String& name
  */
  void
  addRenderTarget(const SPtr<Texture2D>& pRTV, const String& name);

  /**
  *  @brief Reserve a space on the renderer with a given name.
  * 
  *  @param String& passName
  */
  void
  createPass(const String& passName);

  /**
  *  @brief Returns a pass with a given name.
  * 
  *  @param String& passName
  * 
  *  @return SPtr<Pass>
  */
  SPtr<Pass>
  getPass(const String& passName);

  /**
  *  @brief Sets a pass in the renderer with a given name.
  * 
  *  @param SPtr<Pass>& pPass
  *  @param String& passName
  */
  void
  setPass(const SPtr<Pass>& pPass, const String& passName);

  /**
  *  @brief Executes the given pass.
  * 
  *  @param String& passName
  */
  void
  makePass(const String& passName);

  /**
  *  @brief Recompile the shaders on the storaged passes.
  */
  void
  recompileShaders();

  /**
  *  @brief Draw Static Mesh Union Components in scene.
  * 
  *  @param Vector<SPtr<StaticMeshUnionComponent>>& meshList
  */
  void
  drawSMUInScene(const Vector<SPtr<StaticMeshUnionComponent>>& meshList);

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
  *  @brief Clean the shader resource view slots.
  *
  *  @param uint32 numSRV = 128
  */
  void
  cleanShaderResourceView(uint32 numSRV = 128);

  /**
  *  @brief Clean the vertex shader constant buffer slots.
  *
  *  @param uint32 numCB = 14
  */
  void
  cleanVSConstantBuffers(uint32 numCB = 14);

  /**
  *  @brief Clean the pixel shader constant buffer slots.
  *
  *  @param uint32 numCB = 14
  */
  void
  cleanPSConstantBuffers(uint32 numCB = 14);

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
  *  @param Vector2& screenD
  */
  void
  setScreenDimensions(const Vector2& screenD);

 private:
  /**
  *  @brief Map to save passes.
  */
  UMap<String, SPtr<Pass>> m_passes;

  /**
  *  @brief Map to save targets.
  */
  UMap<String, SPtr<Texture2D>> m_targets;

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

  /**
  *  @brief Shadow map texture size.
  */
  float m_sMapSize = 0.0f;

  /**
  *  @brief Screen size.
  */
  Vector2 m_screenDimension = { 0.0f, 0.0f };
};

/**
*  @brief Easier way to access the RendererManager module.
*/
SH_CORE_EXPORT RenderManager&
g_renderMan();
}
