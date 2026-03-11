/*****************************************************************************/
/*
*  @file    shTransformComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/02/21
*  @brief   Transform Component class.
*
*  Transform Component class.
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
#include "shComponent.h"
#include <shMatrix4.h>
#include <shVector3.h>
#include <shQuaternion.h>

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

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Gets the transform data.
  * 
  *  @return Transform&
  */
  const Matrix4&
  getTransformMatrix();

  /**
  *  @brief Gets the position.
  *
  *  @return Vector3
  */
  const Vector3&
  getPosition() const;

  /**
  *  @brief Gets the rotation.
  *
  *  @return Vector3
  */
  const Quaternion&
  getRotation() const;

  /**
  *  @brief Gets the scale.
  *
  *  @return Vector3
  */
  const Vector3&
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
  setRotation(const Quaternion& rotation);

  /**
  *  @brief Changes the scale.
  *
  *  @param Vector3& scale
  */
  void
  setScale(const Vector3& scale);

 protected:
  void
  onUpdate() override {}

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 private:
  /**
  *  @brief Transform data.
  */
  Matrix4 m_transform = Matrix4::IDENTITY;

  /**
  *  @brief Position vector.
  */
  Vector3 m_position = Vector3::ZERO;

  /**
  *  @brief Rotation vector.
  */
  Quaternion m_rotation = Quaternion::IDENTITY;

  /**
  *  @brief Scale vector.
  */
  Vector3 m_scale = Vector3::ONE;

  bool m_bDirty = false;
};
}
