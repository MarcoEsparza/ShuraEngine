/*************************************************************/
/*
*  @file    shGameObject.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/16
*  @brief   Game Object class.
*
*  Game Object class.
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
#include "shTransformComponent.h"
#include "shVector3.h"

namespace shEngineSDK {
/*************************************************************/
/*
*  Forward declarations
*/
/*************************************************************/
class Component;

/**
*  @brief Game Object class.
*/
class GameObject
{
 public:
  /**
  *  @brief Default constructor.
  */
   GameObject() = default;
  /**
  *  @brief Default destructor.
  */
  ~GameObject() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
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

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
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
};
}
