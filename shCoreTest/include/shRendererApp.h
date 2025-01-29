/*************************************************************/
/*
*  @file    shRendererApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/28
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
#include "shBaseApp.h"
#include "shCamera.h"
#include "shMatrix4.h"
#include "shVector3.h"

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
  ViewMatrix view;
  ProjectionMatrix proj;
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
  onUpdate(float deltaTime) override;

  void
  onRender() override;

  void
  onKeyPressed(const KEY::E key, const ModifierState modifier) override;

  void
  onMouseButtonPressed(const MOUSE_INPUT::E mouseButton, const ModifierState modifier) override;

  void
  onMouseMove(const MouseMoveData& mouse) override;

  void
  initGraphicAssets();

  void
  initCamera();

  void
  moveCamera(const Vector3& direction);

  void
  rotateCamera(const float pitch, const float yaw);

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

  FPSCamera m_camera;
  SPtr<SceneGraph> m_scene;
};
}
