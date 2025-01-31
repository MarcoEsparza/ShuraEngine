/*****************************************************************************/
/*
*  @file    shRendererApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/29
*  @brief   
*
*  
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
#include "shBaseApp.h"
#include "shCamera.h"
#include "shMatrix4.h"
#include "shVector3.h"
#include "shVector2.h"

namespace shEngineSDK {
class ProgramShader;
class InputLayout;
class SamplerState;
class RasterizerState;
class BlendState;
class DepthStencilState;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class SceneGraph;
class PBRMaterial;

struct VP
{
  Matrix4 view;
  Matrix4 proj;
};

/**
*  @brief 
*/
class RendererApp : public BaseApp
{
 public:
  RendererApp() = default;
  RendererApp(const ScreenDesc& desc,
              const GRAPHIC_API::E dllGAPI = GRAPHIC_API::kDX11,
              const SampleDesc& sample = SampleDesc(1, 1))
              : BaseApp(desc, dllGAPI, sample),
                m_desc(desc) {}
  ~RendererApp() = default;

 private:
  void
  onCreate() override;

  void
  onUpdate() override;

  void
  onRender() override;

  void
  onKeyPressed(const KEY::E key, const ModifierState modifier) override;

  void
  onKeyReleased(const KEY::E key, const ModifierState modifier) override;

  void
  onMouseButtonPressed(const MOUSE_INPUT::E mouseButton,
                       const ModifierState modifier) override;

  void
  onMouseButtonReleased(const MOUSE_INPUT::E mouseButton,
                       const ModifierState modifier) override;

  void
  onMouseMove(const MouseMoveData& mouse) override;

  void
  onDestroy() override;

  void
  compileShader();

  void
  initGraphicAssets();

  void
  initCamera();

  void
  rotateCamera();

  void
  updateCamera();

 private:
  ScreenDesc m_desc;
  SPtr<ProgramShader> m_pShader;
  SPtr<InputLayout> m_pInputLayout;
  SPtr<SamplerState> m_pSamplerState;
  SPtr<RasterizerState> m_pRasterState;
  SPtr<BlendState> m_pBlendState;
  SPtr<DepthStencilState> m_pDepthStencilState;
  SPtr<ConstantBuffer> m_pVP;
  SPtr<ConstantBuffer> m_pModelTransform;
  SPtr<VertexBuffer> m_pModelVertexB;
  SPtr<IndexBuffer> m_pModelIndexB;
  SPtr<PBRMaterial> m_pModelMat;

  Camera m_camera;

  bool m_leftClick = false;
  Vector2 m_lastMousePos = Vector2(0.0f, 0.0f);
  Vector2 m_currentMousePos = Vector2(0.0f, 0.0f);

  bool m_foward = false;
  bool m_left = false;
  bool m_back = false;
  bool m_right = false;
  bool m_up = false;
  bool m_down = false;
};
}
