/*************************************************************/
/*
*  @file    shSceneGraph.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/15
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
SPtr<GameObject>
SceneGraph::createEmptyObject(const String& objectName)
{
  auto newObject = make_shared<GameObject>();
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