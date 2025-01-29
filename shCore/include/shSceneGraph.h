/*************************************************************/
/*
*  @file    shSceneGraph.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/15
*  @brief   Scene graph class.
*
*  Scene graph class.
*
*  @bug     No bug known.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shGameObject.h"
#include "shBuffers.h"
#include "shGraphicTypes.h"
#include "shInputLayout.h"

namespace shEngineSDK {
/**
*  @brief Scene graph class.
*/
class SH_CORE_EXPORT SceneGraph
{
 public:
  /**
  *  @brief Default constructor.
  */
  SceneGraph() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~SceneGraph() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
 public:
  /**
  *  @brief Creates and add a empty object to the scene.
  * 
  *  @param String& objectName
  */
  SPtr<GameObject>
  createEmptyObject(const String& objectName);

  /**
  *  @brief Adds a object to the scene.
  * 
  *  @param SPtr<GameObject>& object
  */
  void
  addObject(const SPtr<GameObject>& object);

  /**
  *  @brief Gets the object list.
  * 
  *  @return const Vector<SPtr<GameObject>>&
  */
  const Vector<SPtr<GameObject>>&
  getGameObjectList() const;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
 private:
  /**
  *  @brief Game Objects storage.
  */
  Vector<SPtr<GameObject>> m_gameObjects;
};
}
