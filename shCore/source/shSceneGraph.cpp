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
#include "shFileExplorer.h"

#include "shGameObject.h"
#include "shMeshComponent.h"
#include "shAnimatorComponent.h"
#include "shSkyBoxComponent.h"
#include "shLightComponent.h"
#include "shImageResource.h"
#include <shPath.h>

#define DEFAULT_SKYBOX_NAME "Textures/castel_st_angelo_roof_2k.hdr"

using std::to_string;

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
  newObject->m_name = objectName;
  newObject->m_bActive = true;
  //newObject->m_transform.getTransformMatrix() = Matrix4::IDENTITY;
  //newObject->m_transform.setPosition(Vector3::ZERO);
  //newObject->m_transform.setRotation(Vector3::ZERO);
  //newObject->m_transform.setScale(Vector3::ONE);

  addObject(newObject);

  return newObject;
}

SPtr<GameObject>
SceneGraph::createCubeObject()
{
  ResourceManager& resourceMan = g_resourceMan();

  auto newObject = sh_makeShared<GameObject>();
  newObject->m_name = "Cube";
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
  //ResourceManager& resMan = g_resourceMan();
  //ShaderManager& shaderMan = g_shaderMan();

  // Load default skybox
  addDefaultSkybox();
  // Load default light
  addDefaultDirectionalLight();
}

void
SceneGraph::addDefaultSkybox()
{
  ResourceManager& resMan = g_resourceMan();

  Path skyboxPath(FileExplorer::DEFAULT_ASSETS_FOLDER + DEFAULT_SKYBOX_NAME);
  auto skyboxTx = cast::re_ptr<ImageResource>(resMan.loadResourceFromFile(skyboxPath));
  auto pSkyBox = sh_makeShared<SkyBoxComponent>();
  pSkyBox->setSkyBoxResource(skyboxTx);
  auto pSkyBoxGO = sh_makeShared<GameObject>();
  pSkyBoxGO->addComponent(pSkyBox);
  pSkyBoxGO->m_name = "DefaultSkyBox";
  addObject(pSkyBoxGO);
}

void
SceneGraph::addDefaultDirectionalLight()
{
  //ResourceManager& resMan = g_resourceMan();
  ShaderManager& shaderMan = g_shaderMan();

  auto light = sh_makeShared<GameObject>();
  light->m_name = "DirectionalLight";
  //light->m_transform.getTransformMatrix() = Matrix4::IDENTITY;

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
  /*String objectName = object->m_name;
  uint32 copyCount = 1;
  for (auto& gameObject : m_gameObjects) {
    if (gameObject->m_name == objectName) {
      object->m_name += "_(" + to_string(copyCount) + ")";
      copyCount++;
    }
  }*/
  object->m_name = generateUniqueName(object->m_name);
  m_gameObjects.push_back(object);
}

void
SceneGraph::instantiate(const GameObject& object)
{
  auto newObject = sh_makeShared<GameObject>(object);
  addObject(newObject);
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
    for (auto& component : gameObject->m_componentList) {
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
      for (auto& component : gameObject->m_componentList) {
        component->update();
      }
    }
  }
}

void
SceneGraph::serialize()
{

}

void
SceneGraph::deserialize()
{
}

String
SceneGraph::generateUniqueName(const String& baseName)
{
  String uniqueName = baseName;
  uint32 copyCount = 1;
  bool isUnique = false;
  
  while (!isUnique) {
    isUnique = true;
    for (auto& gameObject : m_gameObjects) {
      if (gameObject->m_name == uniqueName) {
        uniqueName = baseName + "_(" + to_string(copyCount) + ")";
        copyCount++;
        isUnique = false;
        break;
      }
    }
  }
  
  return uniqueName;
}

SceneGraph& g_sceneGraph()
{
  return SceneGraph::instance();
}
}