/*************************************************************/
/*
*  @file    shRendererApp.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/28
*  @brief
*
*
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shRendererApp.h"

#include "shGraphicsManager.h"
#include "shResourceManager.h"
#include "shTime.h"
#include "shSceneGraph.h"
#include "shMath.h"

#include "shPath.h"
#include "shImageResource.h"
#include "shMeshResource.h"
#include "shGameObject.h"
#include "shMeshComponent.h"
#include "shMaterial.h"

#include "shRadian.h"
#include "shVector4.h"

using std::reinterpret_pointer_cast;

namespace shEngineSDK {
void
RendererApp::onCreate()
{
  initGraphicAssets();
  initCamera();

  Path whitePNG("resources/White.png");
  g_resourceMan().loadResourceFromFile(whitePNG);

  Path text1("resources/base_albedo.png");
  auto baseColor = reinterpret_pointer_cast<ImageResource>(
                   g_resourceMan().loadResourceFromFile(text1)); 
  Path text2("resources/base_normal.png");
  auto normal = reinterpret_pointer_cast<ImageResource>(
                g_resourceMan().loadResourceFromFile(text2));

  Path text3("resources/base_metallic.png");
  auto metallic = reinterpret_pointer_cast<ImageResource>(
                  g_resourceMan().loadResourceFromFile(text3));

  Path text4("resources/base_roughness.png");
  auto roughness = reinterpret_pointer_cast<ImageResource>(
                   g_resourceMan().loadResourceFromFile(text4));

  Path text5("resources/base_AO.png");
  auto ao = reinterpret_pointer_cast<ImageResource>(
            g_resourceMan().loadResourceFromFile(text5));

  Path modelPath("resources/DrakeFire.fbx");
  auto modelRes = reinterpret_pointer_cast<StaticMeshUnionResource>(
                  g_resourceMan().loadResourceFromFile(modelPath));

  m_pModelMat = make_shared<PBRMaterial>();
  m_pModelMat->baseColor = baseColor->texture;
  m_pModelMat->normal = normal->texture;
  m_pModelMat->metallic = metallic->texture;
  m_pModelMat->roughness = roughness->texture;
  m_pModelMat->ao = ao->texture;

  Vector<VertexData> vertices;
  Vector<uint32> indices;

  for (auto& mesh : modelRes->meshes) {
    for (auto& vertex : mesh->vertices) {
      vertices.push_back(vertex);
    }
    for (auto& index : mesh->indices) {
      indices.push_back(index);
    }
  }

  m_pModelVertexB = g_graphicsMan().createVertexBuffer(vertices);
  m_pModelIndexB = g_graphicsMan().createIndexBuffer(indices);

  auto modelGO = make_shared<GameObject>();
  auto modelMC = make_shared<StaticMeshUnionComponent>();

  modelMC->meshesData = modelRes;
  modelGO->addComponent(modelMC);

  modelGO->transform.getTransform().m[3][3] = 1.0f;

  modelGO->setPosition(Vector3(0.0f, 0.0f, 0.0f));
  modelGO->setScale(Vector3(1.0f, 1.0f, 1.0f));
  modelGO->setRotation(Vector3(0.0f, 90.0f, 0.0f));

  g_sceneGraph().addObject(modelGO);

  m_pModelTransform = g_graphicsMan().createConstantBuffer(sizeof(Transform));

  g_graphicsMan().updateConstantBuffer(m_pModelTransform,
                                       &modelGO->transform.getTransform(),
                                       sizeof(Transform));

  m_light.position = Vector3(1.0f, 1.0f, 1.0f);
  m_light.position.normalize();
  m_light.color = LinearColor(1.0f, 1.0f, 1.0f);
  m_light.intensity = 1.0f;

  m_pLightBuffer = g_graphicsMan().createConstantBuffer(sizeof(Light));

  g_graphicsMan().updateConstantBuffer(m_pLightBuffer, &m_light, sizeof(Light));
}

void
RendererApp::onUpdate()
{
  if (m_leftClick) {
    rotateCamera();
  }

  const float speed = 0.01f;

  if (m_foward) {
    m_camera.move(Vector3(0.0f, 0.0f, 0.1f) * speed);
  }
  
  if (m_left) {
    m_camera.move(Vector3(-0.1f, 0.0f, 0.0f) * speed);
  }

  if (m_back) {
    m_camera.move(Vector3(0.0f, 0.0f, -0.1f) * speed);
  }
  
  if (m_right) {
    m_camera.move(Vector3(0.1f, 0.0f, 0.0f) * speed);
  }

  if (m_up) {
    m_camera.move(Vector3(0.0f, 0.1f, 0.0f) * speed);
  }

  if (m_down) {
    m_camera.move(Vector3(0.0f, -0.1f, 0.0f) * speed);
  }

  auto& goList = g_sceneGraph().getGameObjectList();
  
  const float rotSpeed = 15.0f;
  const float rotAngle = rotSpeed * g_time().getFrameDeltaTime() * Math::DEG2RAD;
  if (m_rotLeft) {
    goList[0]->rotate(Vector3(0.0f, 1.0f, 0.0f), -rotAngle);
    g_graphicsMan().updateConstantBuffer(m_pModelTransform,
                                         &goList[0]->transform.getTransform(),
                                         sizeof(Transform));
  }
  if (m_rotRight) {
    goList[0]->rotate(Vector3(0.0f, 1.0f, 0.0f), rotAngle);
    g_graphicsMan().updateConstantBuffer(m_pModelTransform,
                                         &goList[0]->transform.getTransform(),
                                         sizeof(Transform));
  }
  if (m_rotUp) {
    goList[0]->rotate(Vector3(1.0f, 0.0f, 0.0f), rotAngle);
    g_graphicsMan().updateConstantBuffer(m_pModelTransform,
                                         &goList[0]->transform.getTransform(),
                                         sizeof(Transform));
  }
  if (m_rotDown) {
    goList[0]->rotate(Vector3(1.0f, 0.0f, 0.0f), -rotAngle);
    g_graphicsMan().updateConstantBuffer(m_pModelTransform,
                                         &goList[0]->transform.getTransform(),
                                         sizeof(Transform));
  }
  
  m_camera.update();
  updateCamera();
}

void
RendererApp::onRender()
{
  g_graphicsMan().setRenderTargets(g_graphicsMan().getMainRenderTargetView(),
                                   g_graphicsMan().getMainDepthStencil(),
                                   1);
  
  m_pBasicShader->setPass();
  g_graphicsMan().setPrimitiveTopology();

  g_graphicsMan().vsSetConstantBuffers(m_pVP);
  g_graphicsMan().vsSetConstantBuffers(m_pModelTransform, 1);
  g_graphicsMan().vsSetConstantBuffers(m_pCameraFoward, 2);
  g_graphicsMan().vsSetConstantBuffers(m_pLightBuffer, 3);

  g_graphicsMan().setVertexBuffers(m_pModelVertexB);
  g_graphicsMan().setIndexBuffers(m_pModelIndexB);
  g_graphicsMan().setShaderResourceView(m_pModelMat->baseColor);
  g_graphicsMan().setShaderResourceView(m_pModelMat->normal, 1);
  g_graphicsMan().setShaderResourceView(m_pModelMat->metallic, 2);
  g_graphicsMan().setShaderResourceView(m_pModelMat->roughness, 3);
  g_graphicsMan().setShaderResourceView(m_pModelMat->ao, 4);

  auto& goList = g_sceneGraph().getGameObjectList();
  for (auto& gObject : goList) {
    for (auto& component : gObject->components) {
      if (component->getType() == COMPONENT_TYPE::kStaticMeshUnion) {
        auto meshUC = reinterpret_pointer_cast<StaticMeshUnionComponent>(component);
        uint32 indexCount = 0;
        uint32 vertexCount = 0;
        for (auto mesh : meshUC->meshesData->meshes) {
          g_graphicsMan().drawIndexed(mesh->numIndex,
                                      indexCount,
                                      vertexCount);
          indexCount += mesh->numIndex;
          vertexCount += mesh->numVertex;
        }
      }
    }
  }
}

void
RendererApp::onKeyPressed(const KEY::E key, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);

  if (key == KEY::kW) {
    m_foward = true;
  }

  if (key == KEY::kA) {
    m_left = true;
  }

  if (key == KEY::kS) {
    m_back = true;
  }

  if (key == KEY::kD) {
    m_right = true;
  }

  if (key == KEY::kQ) {
    m_down = true;
  }

  if (key == KEY::kE) {
    m_up = true;
  }

  if (key == KEY::kUp) {
    m_rotUp = true;
  }

  if (key == KEY::kDown) {
    m_rotDown = true;
  }

  if (key == KEY::kLeft) {
    m_rotLeft = true;
  }

  if (key == KEY::kRight) {
    m_rotRight = true;
  }
}

void
RendererApp::onKeyReleased(const KEY::E key, const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);

  if (key == KEY::kW) {
    m_foward = false;
  }

  if (key == KEY::kA) {
    m_left = false;
  }

  if (key == KEY::kS) {
    m_back = false;
  }

  if (key == KEY::kD) {
    m_right = false;
  }

  if (key == KEY::kQ) {
    m_down = false;
  }

  if (key == KEY::kE) {
    m_up = false;
  }

  if (key == KEY::kUp) {
    m_rotUp = false;
  }

  if (key == KEY::kDown) {
    m_rotDown = false;
  }

  if (key == KEY::kLeft) {
    m_rotLeft = false;
  }

  if (key == KEY::kRight) {
    m_rotRight = false;
  }

  if (key == KEY::kC) {
    m_pBasicShader->compileShader();
  }
}

void
RendererApp::onMouseButtonPressed(const MOUSE_INPUT::E mouseButton,
                                  const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);

  if (mouseButton == MOUSE_INPUT::kLeft)
  {
    m_leftClick = true;
  }
}

void
RendererApp::onMouseButtonReleased(const MOUSE_INPUT::E mouseButton,
                                   const ModifierState modifier)
{
  SH_UNREFERENCED_PARAMETER(modifier);

  if (mouseButton == MOUSE_INPUT::kLeft)
  {
    m_leftClick = false;
  }
}

void
RendererApp::onMouseMove(const MouseMoveData& mouse)
{
  m_lastMousePos = m_currentMousePos;
  m_currentMousePos.x = static_cast<float>(mouse.x);
  m_currentMousePos.y = static_cast<float>(mouse.y);
}

void
RendererApp::onDestroy()
{
  
}

void
RendererApp::initGraphicAssets()
{
  setBackgroundColor(LinearColor(0.0f, 0.0f, 0.0f));

  m_pBasicShader = make_unique<Pass>();

  m_pBasicShader->setShaderInfo("resources/BasicShader.hlsl",
                                "main",
                                "mainPS",
                                "vs_5_0",
                                "ps_5_0");
  m_pBasicShader->compileShader();

  RasterizerDesc rasterDesc = {};
  rasterDesc.fillMode = FILL_MODE::kSolid;
  rasterDesc.cullMode = CULL_MODE::kNone;
  rasterDesc.frontCounterClockwise = false;
  rasterDesc.depthBias = 0;
  rasterDesc.depthBiasClamp = 0.0f;
  rasterDesc.slopeScaledDepthBias = 0.0f;
  rasterDesc.depthClipEnable = true;
  rasterDesc.scissorEnable = false;
  rasterDesc.multisampleEnable = false;
  rasterDesc.antialiasedLineEnable = false;

  BlendDesc blendDesc = {};
  blendDesc.renderTarget[0].blendEnable = true;
  blendDesc.renderTarget[0].srcBlend = BLEND::kOne;
  blendDesc.renderTarget[0].destBlend = BLEND::kZero;
  blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
  blendDesc.renderTarget[0].destBlendAlpha = BLEND::kZero;
  blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;

  DepthStencilDesc depthSDesc = {};
  depthSDesc.depthEnable = false;
  depthSDesc.depthWriteMask = DEPTH_WRITE_MASK::kAll;
  depthSDesc.depthFunc = COMPARISON_FUNC::kAlways;
  depthSDesc.stencilEnable = false;
  depthSDesc.stencilReadMask = 0;
  depthSDesc.stencilWriteMask = 0;
  DepthStencilOpDesc frontOPDesc = {};
  frontOPDesc.stencilFailOp = STENCIL_OP::kZero;
  frontOPDesc.stencilDepthFailOp = STENCIL_OP::kZero;
  frontOPDesc.stencilPassOp = STENCIL_OP::kZero;
  frontOPDesc.stencilFunc = COMPARISON_FUNC::kNever;
  DepthStencilOpDesc backOPDesc = {};
  backOPDesc.stencilFailOp = STENCIL_OP::kZero;
  backOPDesc.stencilDepthFailOp = STENCIL_OP::kZero;
  backOPDesc.stencilPassOp = STENCIL_OP::kZero;
  backOPDesc.stencilFunc = COMPARISON_FUNC::kNever;
  depthSDesc.frontFace = frontOPDesc;
  depthSDesc.backFace = backOPDesc;

  m_pBasicShader->generateInputLayout();
  auto pSamplerLinear = g_graphicsMan().createSamplerState();
  m_pBasicShader->setSamplerState(pSamplerLinear);
  m_pBasicShader->setRasterizerState(rasterDesc);
  m_pBasicShader->setBlendState(blendDesc);
  //m_pBasicShader->setDepthStencilState(depthSDesc);
}

void
RendererApp::initCamera()
{
  m_pVP = g_graphicsMan().createConstantBuffer(sizeof(VP));

  VP vp;

  m_camera = Camera(Vector3(0.0f, 0.0f, -2.0f),
                    Vector3(0.0f, 0.0f, 0.0f),
                    Vector3::UP,
                    45.0f * Math::DEG2RAD,
                    static_cast<float>(m_desc.width),
                    static_cast<float>(m_desc.height),
                    0.1f,
                    100.0f);

  vp.proj = m_camera.getProjection();
  vp.view = m_camera.getView();

  vp.proj.getTransposed();
  vp.view.getTransposed();

  g_graphicsMan().updateConstantBuffer(m_pVP, &vp, sizeof(vp));

  Vector4 foward(m_camera.getPosition(), 0.0f);
  
  m_pCameraFoward = g_graphicsMan().createConstantBuffer(sizeof(Vector4));

  g_graphicsMan().updateConstantBuffer(m_pCameraFoward,
                                       &foward,
                                       sizeof(Vector4));
}

void
RendererApp::rotateCamera()
{
  const float speed = 0.005f;

  const float dx = (m_lastMousePos.x - m_currentMousePos.x) * speed;
  const float dy = (m_lastMousePos.y - m_currentMousePos.y) * speed;

  if (m_lastMousePos.x != m_currentMousePos.x ||
      m_lastMousePos.y != m_currentMousePos.y) {
    m_camera.rotate(dx * Math::DEG2RAD, dy * Math::DEG2RAD);
    /*m_camera.orbitCamera(Radian(dx * Math::DEG2RAD),
                         Radian(dy * Math::DEG2RAD),
                         Vector3::ZERO);*/
  }
}

void
RendererApp::updateCamera()
{
  VP vp;

  vp.proj = m_camera.getProjection();
  vp.view = m_camera.getView();

  vp.proj.getTransposed();
  vp.view.getTransposed();

  g_graphicsMan().updateConstantBuffer(m_pVP, &vp, sizeof(vp));

  Vector4 foward(m_camera.getPosition(), 0.0f);

  g_graphicsMan().updateConstantBuffer(m_pCameraFoward,
                                       &foward,
                                       sizeof(Vector4));
}
}
