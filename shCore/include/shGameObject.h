/*****************************************************************************/
/*
*  @file    shGameObject.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/17
*  @brief   Game Object class.
*
*  Game Object class.
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
#include "shTransformComponent.h"
#include <shVector3.h>

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Forward declarations
*/
/*****************************************************************************/

class Component;

/**
*  @brief Game Object class.
*/
class SH_CORE_EXPORT GameObject
{
 public:
  /**
  *  @brief Default constructor.
  */
   GameObject() = default;
  /**
  *  @brief Default destructor.
  */
  ~GameObject();

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Adds component to the component vector.
  * 
  *  @param SPtr<Component>& comp
  */
  void
  addComponent(const SPtr<Component>& comp);

  /**
  *  @brief Removes a component from the component vector.
  *
  *  @param SPtr<Component>& comp
  */
  void
  removeComponent(const SPtr<Component>& comp);

  /**
  *  @brief Adds a child.
  *
  *  @param SPtr<GameObject>& child
  */
  void
  addChild(const SPtr<GameObject>& child);

  /**
  *  @brief Remove a child.
  *
  *  @param SPtr<GameObject>& child
  */
  void
  removeChild(const SPtr<GameObject>& child);

  /**
  *  @brief Gets the GameObject position.
  *
  *  @return Vector3
  */
  Vector3
  getPosition() const;

  /**
  *  @brief Gets the GameObject rotation.
  *
  *  @return Vector3
  */
  Vector3
  getRotation() const;

  /**
  *  @brief Gets the GameObject scale.
  *
  *  @return Vector3
  */
  Vector3
  getScale() const;

  /**
  *  @brief Changes the GameObject position.
  *
  *  @param Vector3& v
  */
  void
  setPosition(const Vector3& position);

  /**
  *  @brief Changes the GameObject rotation.
  *
  *  @param Vector3& rotation
  */
  void
  setRotation(const Vector3& rotation);

  /**
  *  @brief Changes the GameObject scale.
  *
  *  @param Vector3& scale
  */
  void
  setScale(const Vector3& scale);

  /**
  *  @brief Move the game object, adding a given position.
  *
  *  @param Vector3& position
  */
  void
  move(const Vector3& position);

  /**
  *  @brief Rotate the game object, adding a given rotation.
  *
  *  @param Vector3& rotation
  *  @param float angle
  */
  void
  rotate(const Vector3& rotation, float angle);

  /**
  *  @brief Scale the game object, adding a given new scale.
  *
  *  @param Vector3& scale
  */
  void
  scale(const Vector3& scale);

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Game Object name.
  */
  String name;

  /**
  *  @brief Transform component.
  */
  TransformComponent transform;

  /**
  *  @brief Component vector.
  */
  Vector<SPtr<Component>> components;

  /**
  *  @brief Childs vector.
  */
  Vector<SPtr<GameObject>> childs;

  WPtr<GameObject> parent;

  bool m_bActive = true;
};
}
