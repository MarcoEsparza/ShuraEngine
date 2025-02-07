/*****************************************************************************/
/*
*  @file    shRendererApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/07
*  @brief   App for render testing.
*
*  App for render testing.
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
/*****************************************************************************/
/*
*  Foward declarations
*/
/*****************************************************************************/

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

/**
*  @brief Structure for view-projection.
*/
struct VP
{
  Matrix4 view;
  Matrix4 proj;
};

/**
*  @brief Structure for inverse view-projection.
*/
struct InvVP
{
  Matrix4 invVP;
  Matrix4 invV;
};

/**
*  @brief App for render testing.
*/
class RendererApp : public BaseApp
{
 public:
  /**
  *  @brief Default constructor.
  */
  RendererApp() = default;

  /**
  *  @brief Constructor that initialize the app.
  * 
  *  @param ScreenDesc& desc
  *  @param GRAPHIC_API::E dllGAPI = GRAPHIC_API::kDX11
  *  @param SampleDesc& sample = SampleDesc(1, 1)
  */
  RendererApp(const ScreenDesc& desc,
              const GRAPHIC_API::E dllGAPI = GRAPHIC_API::kDX11,
              const SampleDesc& sample = SampleDesc(1, 1))
              : BaseApp(desc, dllGAPI, sample),
                m_desc(desc) {}

  /**
  *  @brief Default destructor.
  */
  ~RendererApp() = default;

 private:
  /**
  *  @brief Override function to add functionallity on app creation.
  */
  void
  onCreate() override;

  /**
  *  @brief Override function to add functionallity when app updates.
  */
  void
  onUpdate() override;

  /**
  *  @brief Override function to add functionallity on app render.
  */
  void
  onRender() override;

  /**
  *  @brief Override function to add functionallity when a key is pressed.
  * 
  *  @param KEY::E key
  *  @param ModifierState modifier
  */
  void
  onKeyPressed(const KEY::E key, const ModifierState modifier) override;

  /**
  *  @brief Override function to add functionallity when a key is released.
  *
  *  @param KEY::E key
  *  @param ModifierState modifier
  */
  void
  onKeyReleased(const KEY::E key, const ModifierState modifier) override;

  /**
  *  @brief Override function to add functionallity when a mouse button is pressed.
  *
  *  @param MOUSE_INPUT::E mouseButton
  *  @param ModifierState modifier
  */
  void
  onMouseButtonPressed(const MOUSE_INPUT::E mouseButton,
                       const ModifierState modifier) override;

  /**
  *  @brief Override function to add functionallity when a mouse button is released.
  *
  *  @param MOUSE_INPUT::E mouseButton
  *  @param ModifierState modifier
  */
  void
  onMouseButtonReleased(const MOUSE_INPUT::E mouseButton,
                       const ModifierState modifier) override;

  /**
  *  @brief Override function to add functionallity when a mouse move.
  *
  *  @param MouseMoveData& mouse
  */
  void
  onMouseMove(const MouseMoveData& mouse) override;

  /**
  *  @brief Override function to add functionallity on app destroy
  */
  void
  onDestroy() override;

  /**
  *  @brief Initialize the graphic assets.
  */
  void
  initGraphicAssets();

  /**
  *  @brief Initialize the app camera.
  */
  void
  initCamera();

  /**
  *  @brief Rotate the camera.
  */
  void
  rotateCamera();

  /**
  *  @brief Update the camera and the camera buffers.
  */
  void
  updateCamera();

 private:
  /**
  *  @brief Screnn descriptor.
  */
  ScreenDesc m_desc;

  /**
  *  @brief First pass.
  */
  UPtr<Pass> m_pBasicShader;

  /**
  *  @brief Second pass.
  */
  UPtr<Pass> m_pDeferredShader;

  /**
  *  @brief Targets for first pass.
  */
  Vector<SPtr<Texture2D>> m_targets;

  /**
  *  @brief Main render target.
  */
  Vector<SPtr<Texture2D>> m_mainTarget;

  /**
  *  @brief Model game object.
  */
  SPtr<GameObject> m_pModel;

  /**
  *  @brief Constant Buffer for view-projection.
  */
  SPtr<ConstantBuffer> m_pVP;

  /**
  *  @brief Constant Buffer for inverse view-projection
  */
  SPtr<ConstantBuffer> m_pInvVP;

  /**
  *  @brief Constant Buffer for model transform.
  */
  SPtr<ConstantBuffer> m_pModelTransform;

  /**
  *  @brief Constant Buffer for camera position.
  */
  SPtr<ConstantBuffer> m_pCameraPosition;

  /**
  *  @brief Constant Buffer for light struct.
  */
  SPtr<ConstantBuffer> m_pLightBuffer;

  /**
  *  @brief Constant Buffer for viewport.
  */
  SPtr<ConstantBuffer> m_pViewportBuffer;

  /**
  *  @brief App camera.
  */
  Camera m_camera;

  /**
  *  @brief Is left mouse button clicked?
  */
  bool m_bLeftClick = false;

  /**
  *  @brief Last mouse position.
  */
  Vector2 m_lastMousePos = Vector2(0.0f, 0.0f);

  /**
  *  @brief Current mouse position.
  */
  Vector2 m_currentMousePos = Vector2(0.0f, 0.0f);

  /**
  *  @brief Is camera going foward?
  */
  bool m_bFoward = false;

  /**
  *  @brief Is camera going left?
  */
  bool m_bLeft = false;

  /**
  *  @brief Is camera going back?
  */
  bool m_bBack = false;

  /**
  *  @brief Is camera going right?
  */
  bool m_bRight = false;

  /**
  *  @brief Is camera going up?
  */
  bool m_bUp = false;

  /**
  *  @brief Is camera going down?
  */
  bool m_bDown = false;

  /**
  *  @brief Is model rotating right?
  */
  bool m_bRotRight = false;

  /**
  *  @brief Is model rotating left?
  */
  bool m_bRotLeft = false;

  /**
  *  @brief Is model rotating up?
  */
  bool m_bRotUp = false;

  /**
  *  @brief Is model rotating down?
  */
  bool m_bRotDown = false;

  /**
  *  @brief Light structure.
  */
  Light m_light = {};
};
}
