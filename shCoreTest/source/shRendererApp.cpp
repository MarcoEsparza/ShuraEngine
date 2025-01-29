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
#include "shMath.h"

#include "shPath.h"
#include "shImageResource.h"
#include "shMeshResource.h"
#include "shSceneGraph.h"
#include "shGameObject.h"
#include "shMeshComponent.h"
#include "shMaterial.h"

using std::reinterpret_pointer_cast;

namespace shEngineSDK {
void
RendererApp::onCreate()
{
  initGraphicAssets();
  initCamera();

  m_scene = make_shared<SceneGraph>();

  Path whitePNG("resources/White.png");
  g_resourceMan().loadResourceFromFile(whitePNG);

  Path text1("resources/base_albedo.png");
  auto baseColor = reinterpret_pointer_cast<ImageResource>(
                   g_resourceMan().loadResourceFromFile(text1)); 
  Path text2("resources/base_normal.png");
  auto normal = reinterpret_pointer_cast<ImageResource>(
                   g_resourceMan().loadResourceFromFile(text2));

  Path modelPath("resources/DrakeFire.fbx");
  auto modelRes = reinterpret_pointer_cast<StaticMeshUnionResource>(
                  g_resourceMan().loadResourceFromFile(modelPath));

  m_pModelMat = make_shared<PBRMaterial>();
  m_pModelMat->baseColor = baseColor->texture;
  m_pModelMat->normal = normal->texture;

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

  m_scene->addObject(modelGO);

  m_pModelTransform = g_graphicsMan().createConstantBuffer(sizeof(Transform));

  g_graphicsMan().updateConstantBuffer(m_pModelTransform,
                                       &modelGO->transform.getTransform(),
                                       sizeof(Transform));
}

void
RendererApp::onUpdate()
{
  
}

void
RendererApp::onRender()
{
  g_graphicsMan().setRenderTargets(g_graphicsMan().getMainRenderTargetView(),
                                   g_graphicsMan().getMainDepthStencil(),
                                   1);
  g_graphicsMan().setSamplerState(m_pSamplerState);
  g_graphicsMan().setRasterizerState(m_pRasterState);
  g_graphicsMan().setBlendState(m_pBlendState);

  g_graphicsMan().setProgramShader(m_pShader);
  g_graphicsMan().setPrimitiveTopology();
  g_graphicsMan().setInputLayout(m_pInputLayout);
  g_graphicsMan().vsSetConstantBuffers(m_pVP);
  g_graphicsMan().vsSetConstantBuffers(m_pModelTransform, 1);
  g_graphicsMan().setVertexBuffers(m_pModelVertexB);
  g_graphicsMan().setIndexBuffers(m_pModelIndexB);
  g_graphicsMan().setShaderResourceView(m_pModelMat->baseColor);
  g_graphicsMan().setShaderResourceView(m_pModelMat->normal, 1);

  auto& goList = m_scene->getGameObjectList();
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
  if (key == KEY::kW) {

  }

  if (key == KEY::kA) {

  }

  if (key == KEY::kS) {

  }

  if (key == KEY::kD) {

  }
}

void
RendererApp::onMouseButtonPressed(const MOUSE_INPUT::E mouseButton, const ModifierState modifier)
{

}

void
RendererApp::onMouseMove(const MouseMoveData& mouse)
{

}

void
RendererApp::initGraphicAssets()
{
  setBackgroundColor(LinearColor(0.5f, 0.5f, 1.0f));

  Path shaderPath("resources/BasicShader.hlsl");
  m_pShader = g_graphicsMan().createProgramShader(shaderPath.toString(),
                                      "main",
                                      "mainPS",
                                      "vs_5_0",
                                      "ps_5_0");

  Vector<InputDesc> ilDesc;
  ilDesc.resize(3);
  ilDesc[0].type = INPUT_LAYOUT_TYPES::kPosition;
  ilDesc[0].format = TEXTURE_FORMAT::kR32G32B32_float;
  ilDesc[0].size = sizeof(float) * 3;

  ilDesc[1].type = INPUT_LAYOUT_TYPES::kNormal;
  ilDesc[1].format = TEXTURE_FORMAT::kR32G32B32_float;
  ilDesc[1].size = sizeof(float) * 3;

  ilDesc[2].type = INPUT_LAYOUT_TYPES::kTexcoord;
  ilDesc[2].format = TEXTURE_FORMAT::kR32G32_float;
  ilDesc[2].size = sizeof(float) * 2;

  m_pInputLayout = g_graphicsMan().createInputLayout(ilDesc, m_pShader);

  m_pSamplerState = g_graphicsMan().createSamplerState();

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

  m_pRasterState = g_graphicsMan().createRasterizerState(rasterDesc);

  BlendDesc blendDesc = {};
  blendDesc.renderTarget[0].blendEnable = true;
  blendDesc.renderTarget[0].srcBlend = BLEND::kOne;
  blendDesc.renderTarget[0].destBlend = BLEND::kZero;
  blendDesc.renderTarget[0].blendOp = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].srcBlendAlpha = BLEND::kOne;
  blendDesc.renderTarget[0].destBlendAlpha = BLEND::kZero;
  blendDesc.renderTarget[0].blendOpAlpha = BLEND_OP::kAdd;
  blendDesc.renderTarget[0].renderTargetWriteMask = COLOR_WHITE_ENABLE::kEnableAll;

  m_pBlendState = g_graphicsMan().createBlendState(blendDesc);

  
}

void
RendererApp::initCamera()
{
  m_pVP = g_graphicsMan().createConstantBuffer(sizeof(VP));

  VP vp;

  m_camera.setProjectionData(45.0f, m_desc.width, m_desc.height, 0.1f, 100.0f);

  Vector3 eye(0.0f, 0.0f, -2.0f);
  Vector3 at(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);

  m_camera.setViewData(eye, at, up);

  vp.proj = m_camera.getProjection();
  vp.view = m_camera.getView();

  vp.proj.getTransposed();
  vp.view.getTransposed();

  g_graphicsMan().updateConstantBuffer(m_pVP, &vp, sizeof(vp));
}

void
RendererApp::moveCamera(const Vector3& direction)
{

}

void
RendererApp::rotateCamera(const float pitch, const float yaw)
{

}
}
