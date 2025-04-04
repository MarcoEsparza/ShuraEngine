/*****************************************************************************/
/*
*  @file    shRendererApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/28
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
class Sound;

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
              : BaseApp(desc, dllGAPI, sample) {}

  /**
  *  @brief Default destructor.
  */
  ~RendererApp() = default;

  /***************************************************************************/
  /*
  *  Event functions
  */
  /***************************************************************************/
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
  *  @brief Override to add functionality when this event pops.
  */
  virtual void
  onResize(const ResizeData& rszData) override;

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
  *  @brief Override to set mouse wheel event. This will be called every time
  *         the mouse wheel is used.
  */
  void
  onMouseWheel(const double delta, const ModifierState modifier) override;

  /**
  *  @brief Override to set mouse wheel event. This will be called every time
  *         the mouse wheel is used.
  */
  void
  onMouseHWheel(const double delta, const ModifierState modifier) override;

  /**
  *  @brief Override function to add functionallity on app destroy
  */
  void
  onDestroy() override;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
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

  void
  setRenderTargets();

  /**
  *  @brief Sets the graphic interface.
  */
  void
  setImgui();

  void
  loadPistol();

  void
  loadSponza();

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 private:
  /**
  *  @brief Targets for first pass.
  */
  SPtr<Texture2D> m_pDepthTarget;
  SPtr<Texture2D> m_pNormalTarget;
  SPtr<Texture2D> m_pColorTarget;

  /**
  *  @brief Main render target.
  */
  SPtr<Texture2D> m_mainTarget;
  
  /**
  *  @brief Ambient occlusion render target.
  */
  SPtr<Texture2D> m_pAoTarget;

  /**
  *  @brief Horizontal blur render target.
  */
  SPtr<Texture2D> m_pHbTarget;

  /**
  *  @brief Vertical Blur render target.
  */
  SPtr<Texture2D> m_pVbTarget;

  SPtr<Texture2D> m_pSMapTarget;

  /**
  *  @brief Model game object.
  */
  SPtr<GameObject> m_pModel;
  SPtr<GameObject> m_pSponza;

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
  SPtr<ConstantBuffer> m_pSponzaTransform;

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
  *  @brief Constant buffer for ao data.
  */
  SPtr<ConstantBuffer> m_pAOBuffer;


  SPtr<ConstantBuffer> m_pLCBuffer;
  SPtr<ConstantBuffer> m_pLSizeBuffer;

  /**
  *  @brief App camera.
  */
  Camera m_camera;


  Camera m_lightCam;

  /**
  *  @brief Is left mouse button clicked?
  */
  bool m_bLeftClick = false;

  /**
  *  @brief Is right mouse button clicked?
  */
  bool m_bRightClick = false;

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
  *  @brief Mouse wheel delta.
  */
  float m_delta = 0.0f;

  /**
  *  @brief Mouse horizontal wheel delta.
  */
  float m_hdelta = 0.0f;

  /**
  *  @brief Model position.
  */
  Vector3 m_modelPos = { 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Model rotation.
  */
  Vector3 m_modelRot = { 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Model scale.
  */
  Vector3 m_modelScale = { 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Model position.
  */
  Vector3 m_sponzaPos = { 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Model rotation.
  */
  Vector3 m_sponzaRot = { 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Model scale.
  */
  Vector3 m_sponzaScale = { 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Ambient occlusion sampler rad.
  */
  float m_aoSamplerRad = 1.0f;

  /**
  *  @brief Ambient occlusion scale.
  */
  float m_aoScale = 1.0f;

  /**
  *  @brief Ambient occlusion bias.
  */
  float m_aoBias= 1.0f;

  /**
  *  @brief Ambient occlusion intensity.
  */
  float m_aoIntensity = 1.0f;

  /**
  *  @brief Float to test imgui.
  */
  float m_lIntensity = 0.0f;

  /**
  *  @brief String to test imgui.
  */
  String m_str;

  Vector4 m_lightPos = { 0.0f, 0.0f, 0.0f, 0.0f };
  Vector3 m_lightTarget = { 0.0f, 0.0f, 0.0f };

  float m_lcamNear = 0.0f;
  float m_lcamFar = 0.0f;
  float m_lcamSize = 0.0f;

  SPtr<Sound> m_testSound;

  bool bIsSoundPlaying = false;

  float m_shadowTexSize = 0;

  /**
  *  @brief Light structure.
  */
  //Light m_light = {};
};
}
