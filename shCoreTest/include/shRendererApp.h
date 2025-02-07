/*****************************************************************************/
/*
*  @file    shRendererApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/04
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
#include "shLight.h"
#include "shPass.h"

namespace shEngineSDK {
class RenderTargetView;
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
class GameObject;
class Texture2D;

struct VP
{
  Matrix4 view;
  Matrix4 proj;
};

struct InvVP
{
  Matrix4 invVP;
  Matrix4 invV;
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
  initGraphicAssets();

  void
  initCamera();

  void
  rotateCamera();

  void
  updateCamera();

 private:
  ScreenDesc m_desc;
  UPtr<Pass> m_pBasicShader;
  UPtr<Pass> m_pDeferredShader;

  Vector<SPtr<Texture2D>> m_targets;

  SPtr<GameObject> m_pModel;

  SPtr<ConstantBuffer> m_pVP;
  SPtr<ConstantBuffer> m_pInvVP;
  SPtr<ConstantBuffer> m_pModelTransform;
  SPtr<ConstantBuffer> m_pCameraFoward;
  SPtr<ConstantBuffer> m_pLightBuffer;
  SPtr<ConstantBuffer> m_pViewportBuffer;

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

  bool m_rotRight = false;
  bool m_rotLeft = false;
  bool m_rotUp = false;
  bool m_rotDown = false;

  float m_horizontalRot = 0.0f;
  float m_verticalRot = 0.0f;

  Light m_light = {};
};
}
