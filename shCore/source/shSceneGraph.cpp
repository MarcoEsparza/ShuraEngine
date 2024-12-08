/*************************************************************/
/*
*  @file    sh.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/09
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
#include "shSceneGraph.h"
#include "shGraphicsManager.h"
#include "shMeshComponent.h"
#include "shAnimatorComponent.h"

using std::reinterpret_pointer_cast;

namespace shEngineSDK {
void
SceneGraph::createEmptyObject(const String& objectName)
{
  auto newObject = make_shared<GameObject>();
  newObject->name = objectName;

  addObject(newObject);
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
}