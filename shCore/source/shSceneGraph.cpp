/*************************************************************/
/*
*  @file    shSceneGraph.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/23
*  @brief   Scene graph class.
*
*  Scene graph class.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shSceneGraph.h"
#include "shGraphicsManager.h"
#include "shResourceManager.h"
#include "shShaderManager.h"
#include "shGameObject.h"
#include "shMeshComponent.h"
#include "shAnimatorComponent.h"
#include "shSkyBoxComponent.h"
#include "shLightComponent.h"
#include "shImageResource.h"
#include <shPath.h>

namespace shEngineSDK {
SceneGraph::~SceneGraph()
{
  for (auto& object : m_gameObjects) {
    object.reset();
  }

  m_gameObjects.clear();
}

void
SceneGraph::onStartUp()
{
}

SPtr<GameObject>
SceneGraph::createEmptyObject(const String& objectName)
{
  auto newObject = sh_makeShared<GameObject>();
  newObject->name = objectName;
  newObject->m_bActive = true;
  newObject->transform.getTransform() = Matrix4::IDENTITY;
  newObject->transform.setPosition(Vector3::ZERO);
  newObject->transform.setRotation(Vector3::ZERO);
  newObject->transform.setScale(Vector3::ONE);

  addObject(newObject);

  return newObject;
}

SPtr<GameObject>
SceneGraph::createCubeObject()
{
  ResourceManager& resourceMan = g_resourceMan();

  auto newObject = sh_makeShared<GameObject>();
  newObject->name = "Cube";
  auto meshComponent = sh_makeShared<StaticMeshComponent>();
  auto pCube = cast::re_ptr<StaticMeshResource>(resourceMan.getResource("cube.fbx"));
  meshComponent->setMeshData(pCube);
  newObject->addComponent(meshComponent);
  addObject(newObject);
  return newObject;
}

void
SceneGraph::createDefaultScene()
{
  ResourceManager& resMan = g_resourceMan();
  ShaderManager& shaderMan = g_shaderMan();

  // Load default skybox
  addDefaultSkybox();
  // Load default light
  addDefaultDirectionalLight();
}

void
SceneGraph::addDefaultSkybox()
{
  ResourceManager& resMan = g_resourceMan();

  Path skyboxPath("resources/textures/castel_st_angelo_roof_2k.hdr");
  auto skyboxTx = cast::re_ptr<ImageResource>(resMan.loadResourceFromFile(skyboxPath));
  auto pSkyBox = sh_makeShared<SkyBoxComponent>();
  pSkyBox->setSkyBoxResource(skyboxTx);
  auto pSkyBoxGO = sh_makeShared<GameObject>();
  pSkyBoxGO->addComponent(pSkyBox);
  pSkyBoxGO->name = "DefaultSkyBox";
  addObject(pSkyBoxGO);
}

void
SceneGraph::addDefaultDirectionalLight()
{
  ResourceManager& resMan = g_resourceMan();
  ShaderManager& shaderMan = g_shaderMan();

  auto light = sh_makeShared<GameObject>();
  light->name = "DirectionalLight";
  light->transform.getTransform() = Matrix4::IDENTITY;

  auto pLightComp = sh_makeShared<LightComponent>();
  pLightComp->m_lightType = LIGHT_TYPE::kDirectional;
  pLightComp->m_color = LinearColor::WHITE;
  pLightComp->m_intensity = 1.0f;
  pLightComp->m_position = Vector4(0.0f, 100.0f, 0.0f, 0.0f);
  pLightComp->m_target = Vector3::ZERO;
  Vector3 lightPos(pLightComp->m_position.x,
                   pLightComp->m_position.y,
                   pLightComp->m_position.z);
  pLightComp->m_lightCamera = Camera(lightPos,
                                     pLightComp->m_target,
                                     Vector3::UP,
                                     1000.0f,
                                     1000.0f,
                                     0.1f,
                                     1000.0f);

  light->addComponent(pLightComp);
  addObject(light);

  // Temporary initialization of light buffer
  // TODO: Change this when multiple lights are implemented
  shaderMan.m_lightData.position = pLightComp->m_position;
  shaderMan.m_lightData.target = pLightComp->m_target;
  shaderMan.m_lightData.intensity = pLightComp->m_intensity;
  shaderMan.m_lightData.color = pLightComp->m_color;
  shaderMan.m_lightData.view = pLightComp->m_lightCamera.getView().getTransposed();
  shaderMan.m_lightData.proj = pLightComp->m_lightCamera.getProjection().getTransposed();
  shaderMan.updateLightCB();
}

void
SceneGraph::addObject(const SPtr<GameObject>& object)
{
  m_gameObjects.push_back(object);
}

Vector<SPtr<GameObject>>&
SceneGraph::getGameObjectList()
{
  return m_gameObjects;
}

const Vector<SPtr<StaticMeshComponent>>
SceneGraph::getStaticMeshComponentInScene() const
{
  Vector<SPtr<StaticMeshComponent>> meshes;

  for (auto& gameObject : m_gameObjects) {
    for (auto& component : gameObject->components) {
      if (component->getType() == COMPONENT_TYPE::kStaticMesh) {
        auto mesh = sh_reinterpretPCast<StaticMeshComponent>(component);
        meshes.push_back(mesh);
      }
    }
  }

  return meshes;
}

void
SceneGraph::update()
{
  for(auto& gameObject : m_gameObjects) {
    if (gameObject->m_bActive) {
      for (auto& component : gameObject->components) {
        component->update();
      }
    }
  }
}

SceneGraph& g_sceneGraph()
{
  return SceneGraph::instance();
}
}