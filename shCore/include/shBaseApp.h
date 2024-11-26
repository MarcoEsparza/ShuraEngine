/*************************************************************/
/*
*  @file    shBaseApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/
*  @brief
*
*
*
*  @bug     No bug known.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shScreen.h"
#include "shGraphicTypes.h"
#include "shGraphicsManager.h"
#include "shMaterial.h"
#include "shSceneGraph.h"
#include "shCamera.h"

namespace shEngineSDK {
class SH_CORE_EXPORT BaseApp
{
 public:
  BaseApp() = default;
  ~BaseApp() = default;

  void
  run(const ScreenDesc& desc, const String& dllStr);

 private:

  void
  updateWorld(float& trsnform);

  void
  handleEvents();

  void
  update();

  void
  render();

  void
  initGraphicAssets();

 private:
  SPtr<Screen> m_mainScreen;
  UMap<String, SPtr<Material>> m_materials;
  SPtr<ProgramShader> m_pStaticShader;
  SPtr<ProgramShader> m_pSkeletalShader;
  SPtr<InputLayout> m_pStaticInputLayout;
  SPtr<InputLayout> m_pSkeletalInputLayout;
  SPtr<VertexBuffer> m_pStaticMeshVertexBuffer;
  SPtr<IndexBuffer> m_pStaticMeshIndexBuffer;
  SPtr<SamplerState> m_pSamplerLinear;
  SPtr<ConstantBuffer> m_pWVP;

  SPtr<ScreenEventHandle> m_eventQueue;
  bool m_appRunning = false;
  FPSCamera m_editorCamera;
  Matrix4 m_world;
};
}
