/*************************************************************/
/*
*  @file    shTransformComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/18
*  @brief   Transform Component class.
*
*  Transform Component class.
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
#include "shComponent.h"
#include "shMatrix4.h"
#include "shVector3.h"

namespace shEngineSDK {
/**
*  @brief Transform Component class.
*/
class SH_CORE_EXPORT TransformComponent : public Component
{
 public:
  /**
  *  @brief Default constructor.
  */
  TransformComponent() : Component(COMPONENT_TYPE::kTransform) {}

  /**
  *  @brief Default destructor.
  */
  ~TransformComponent() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
 public:
  /**
  *  @brief Gets the transform data.
  * 
  *  @return Transform&
  */
  Transform&
  getTransform();

  /**
  *  @brief Gets the position.
  *
  *  @return Vector3
  */
  Vector3
  getPosition() const;

  /**
  *  @brief Gets the rotation.
  *
  *  @return Vector3
  */
  Vector3
  getRotation() const;

  /**
  *  @brief Gets the scale.
  *
  *  @return Vector3
  */
  Vector3
  getScale() const;

  /**
  *  @brief Changes the position.
  *
  *  @param Vector3& v
  */
  void
  setPosition(const Vector3& position);

  /**
  *  @brief Changes the rotation.
  *
  *  @param Vector3& rotation
  */
  void
  setRotation(const Vector3& rotation);

  /**
  *  @brief Changes the scale.
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
 private:
  /**
  *  @brief Transform data.
  */
  Transform m_transform;

  //Vector3 position;
  //Vector3 rotation;
  //Vector3 scale;
};
}
