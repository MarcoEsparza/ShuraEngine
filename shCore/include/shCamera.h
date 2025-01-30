/*****************************************************************************/
/*
*  @file    shCamera.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/29
*  @brief   Engine Camera class.
*
*  Engine Camera class.
*
*  @bug     Camera movement not working properly.
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesCore.h"
#include "shMatrix4.h"
#include "shVector3.h"

namespace shEngineSDK {
/**
*  @brief Engine Camera class.
*/
class SH_CORE_EXPORT Camera
{
 public:
  /**
  *  @brief Default constructor.
  */
  Camera() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~Camera() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Sets view matrix.
  * 
  *  @param Vector3& camPos
  *  @param Vector3& targetPos
  *  @param Vector3& upVector
  */
  void
  setViewData(const Vector3& camPos,
              const Vector3& targetPos,
              const Vector3& upVector);

  /**
  *  @brief Sets perspective projection matrix.
  *
  *  @param float halfFOV
  *  @param float width
  *  @param float height
  *  @param float minZ
  *  @param float maxZ
  */
  void
  setPerspectiveData(const float halfFOV,
                    const float width,
                    const float height,
                    const float minZ,
                    const float maxZ);

  /**
  *  @brief Sets the orthographic projection matrix.
  *
  *  @param const float left
  *  @param const float right
  *  @param const float bottom
  *  @param const float top
  *  @param const float nearZ
  *  @param const float farZ
  */
  void
  setOrthographicProjData(const float left,
                          const float right,
                          const float bottom,
                          const float top,
                          const float nearZ,
                          const float farZ);

  /**
  *  @brief Move camera.
  *
  *  @param Vector3& direction
  */
  void
  move(const Vector3& direction);

  /**
  *  @brief Rotate camera in the X and Y axes.
  * 
  *  @param float yaw
  *  @param float pitch
  */
  void
  rotateCam(const float yaw, const float pitch);

  

  /**
  *  @brief Gets the view matrix.
  *
  *  @return const ViewMatrix&
  */
  FORCEINLINE const Matrix4&
  getView() const { return m_view; }

  /**
  *  @brief Gets the projection matrix.
  *
  *  @return const ViewMatrix&
  */
  FORCEINLINE const Matrix4&
  getProjection() const { return m_proj; }

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 protected:
  /**
  *  @brief The view matrix.
  */
  Matrix4 m_view = Matrix4::IDENTITY;

  /**
  *  @brief The projection matrix.
  */
  Matrix4 m_proj = Matrix4::IDENTITY;

  /**
  *  @brief Camera position.
  */
  Vector3 m_position = Vector3::ZERO;

  /**
  *  @brief Camera look target.
  */
  Vector3 m_target = Vector3::FORWARD;

  /**
  *  @brief Camera up direction vector.
  */
  Vector3 m_up = Vector3::UP;

  bool m_isDirty = false;

  bool m_isOrtho = false;
};
}
