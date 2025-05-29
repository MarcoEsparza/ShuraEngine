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
#include "shMeshComponent.h"
#include "shAnimatorComponent.h"

namespace shEngineSDK {
SceneGraph::~SceneGraph()
{
  for (auto& object : m_gameObjects) {
    object.reset();
  }

  m_gameObjects.clear();
}

SPtr<GameObject>
SceneGraph::createEmptyObject(const String& objectName)
{
  auto newObject = sh_makeShared<GameObject>();
  newObject->name = objectName;

  addObject(newObject);

  return newObject;
}

void
SceneGraph::addObject(const SPtr<GameObject>& object)
{
  m_gameObjects.push_back(object);
}

const Vector<SPtr<GameObject>>&
SceneGraph::getGameObjectList() const
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

SceneGraph& g_sceneGraph()
{
  return SceneGraph::instance();
}
}