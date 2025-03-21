/*****************************************************************************/
/*
*  @file    shRenderManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/05
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

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Foward declarations
*/
/*****************************************************************************/
class StaticMeshUnionComponent;
class PBRMaterial;
class Pass;

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
  setResourceViewFromPBRMaterial(const SPtr<PBRMaterial>& pMat);

 private:
  /**
  *  @brief Map to save passes.
  */
  UMap<String, SPtr<Pass>> m_passes;
};

/**
*  @brief Easier way to access the RendererManager module.
*/
SH_CORE_EXPORT RenderManager&
g_renderMan();
}
