/*****************************************************************************/
/*
*  @file    shSceneGraph.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/10/02
*  @brief   Scene graph class.
*
*  Scene graph class.
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
#include "shModule.h"

namespace shEngineSDK {
class StaticMeshComponent;
class SkeletalMeshComponent;
class GameObject;

/**
*  @brief Scene graph class.
*/
class SH_CORE_EXPORT SceneGraph : public Module<SceneGraph>
{
 public:
  /**
  *  @brief Default constructor.
  */
  SceneGraph() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~SceneGraph();

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  void
  onStartUp() override;

  /**
  *  @brief Creates and add a empty object to the scene.
  * 
  *  @param String& objectName
  */
  SPtr<GameObject>
  createEmptyObject(const String& objectName);

  SPtr<GameObject>
  createCubeObject();

  void
  createDefaultScene();

  void
  addDefaultSkybox();

  void
  addDefaultDirectionalLight();

  /**
  *  @brief Adds a object to the scene.
  * 
  *  @param SPtr<GameObject>& object
  */
  void
  addObject(const SPtr<GameObject>& object);

  void
  instantiate(const GameObject& object);

  /**
  *  @brief Gets the object list.
  * 
  *  @return const Vector<SPtr<GameObject>>&
  */
  Vector<SPtr<GameObject>>&
  getGameObjectList();

  const Vector<SPtr<StaticMeshComponent>>
  getStaticMeshComponentInScene() const;

  void
  update();

  void
  serialize();

  void
  deserialize();

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 private:
  /**
  *  @brief Game Objects storage.
  */
  Vector<SPtr<GameObject>> m_gameObjects;
};

/**
*  @brief Easier way to access the SceneGraph module.
*/
SH_CORE_EXPORT SceneGraph&
g_sceneGraph();
}
