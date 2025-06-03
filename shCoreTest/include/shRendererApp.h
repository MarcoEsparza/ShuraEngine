/*****************************************************************************/
/*
*  @file    shRendererApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/05/26
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

struct ToneMap
{
  Vector2 screenSize;
  float index;
  float lutSize;
};

struct BrightMap
{
  float brightThreshold = 0.0f;
  Vector3 unused = Vector3::ZERO;
};

struct PostProcessValues
{
  float minR = 0.0f;
  float maxR = 0.0f;
  float minG = 0.0f;
  float maxG = 0.0f;
  float minB = 0.0f;
  float maxB = 0.0f;
  Vector2 unused = Vector2(0.0f, 0.0f);
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
 private:
  /**
  *  @brief Initialize the shaders.
  */
  void
  initShaders();

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

  /**
  *  @brief Initialize light orthographic camera.
  */
  void
  initLightCamera();

  /**
  *  @brief Set render targets information.
  */
  void
  setRenderTargets();

  /**
  *  @brief Sets the graphic interface.
  */
  void
  setImgui();

  /**
  *  @brief Load pistol model, materials and game object.
  */
  void
  loadPistol();

  /**
  *  @brief Load sponza model, materials and game object.
  */
  void
  loadSponza();

  /**
  *  @brief Load skybox model, materials and game object.
  */
  void
  loadSkybox();

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 private:
  /**
  *  @brief Pistol game object.
  */
  SPtr<GameObject> m_pModel;

  /**
  *  @brief Sponza game object.
  */
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
  *  @brief Constant Buffer for pistol transform.
  */
  SPtr<ConstantBuffer> m_pModelTransform;

  /**
  *  @brief Constant Buffer for sponza transform.
  */
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

  /**
  *  @brief Constant buffer for light camera.
  */
  SPtr<ConstantBuffer> m_pLCBuffer;

  /**
  *  @brief Constant buffer for light size.
  */
  SPtr<ConstantBuffer> m_pLSizeBuffer;


  SPtr<ConstantBuffer> m_pToneMapBuffer;
  SPtr<ConstantBuffer> m_pLuminanceBuffer;
  SPtr<ConstantBuffer> m_pPPBuffer;

  /**
  *  @brief App camera.
  */
  Camera m_camera;

  /**
  *  @brief Light orthographic camera.
  */
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
  *  @brief Screen size.
  */
  Vector2 m_screenSize = Vector2(0.0f, 0.0f);

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
  *  @brief Pistol position.
  */
  Vector3 m_modelPos = { 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Pistol rotation.
  */
  Vector3 m_modelRot = { 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Pistol scale.
  */
  Vector3 m_modelScale = { 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Sponza position.
  */
  Vector3 m_sponzaPos = { 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Sponza rotation.
  */
  Vector3 m_sponzaRot = { 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Sponza scale.
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
  *  @brief Light position.
  */
  Vector4 m_lightPos = { 0.0f, 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Light camera target.
  */
  Vector3 m_lightTarget = { 0.0f, 0.0f, 0.0f };

  /**
  *  @brief Light camera near.
  */
  float m_lcamNear = 0.0f;

  /**
  *  @brief Light camera far.
  */
  float m_lcamFar = 0.0f;

  /**
  *  @brief Light camera size.
  */
  float m_lcamSize = 0.0f;

  /**
  *  @brief Testing audio.
  */
  SPtr<Sound> m_testSound;

  /**
  *  @brief Is audio playing?
  */
  bool bIsSoundPlaying = false;

  /**
  *  @brief Size of shadow texture.
  */
  float m_shadowTexSize = 0;

  int32 m_toneMapIndex = 0;
  float m_minR = 0.0f;
  float m_maxR = 0.0f;
  float m_minG = 0.0f;
  float m_maxG = 0.0f;
  float m_minB = 0.0f;
  float m_maxB = 0.0f;

  float m_brightT = 1.0f;

  float m_fpsTimer = 0.0f;
  uint32 m_fpsCount = 0;
  uint32 m_fpsCountGUI = 0;
};
}
