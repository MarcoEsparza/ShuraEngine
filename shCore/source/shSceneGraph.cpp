/*************************************************************/
/*
*  @file    shSceneGraph.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/14
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

using std::reinterpret_pointer_cast;

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

const Vector<SPtr<StaticMeshUnionComponent>>
SceneGraph::getStaticMeshUnionComponentInScene() const
{
  Vector<SPtr<StaticMeshUnionComponent>> smuComponents;

  for (auto& object : m_gameObjects) {
    for (auto& component : object->components) {
      if (component->getType() == COMPONENT_TYPE::kStaticMeshUnion) {
        auto meshUC = reinterpret_pointer_cast<StaticMeshUnionComponent>(component);
        smuComponents.push_back(meshUC);
      }
    }
  }

  return smuComponents;
}

SceneGraph& g_sceneGraph()
{
  return SceneGraph::instance();
}
}