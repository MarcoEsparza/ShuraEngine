/*****************************************************************************/
/*
*  @file    shCamera.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/11
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
#include "shRadian.h"
#include "shDegree.h"
#include "shFrustum.h"

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
  *  @brief Perpective camera constructor.
  * 
  *  @param const Vector3& camPos
  *  @param const Vector3& targetPos
  *  @param const Vector3& upVector
  *  @param const float halfFOV
  *  @param const float width
  *  @param const float height
  *  @param const float minZ
  *  @param const float maxZ
  */
  FORCEINLINE Camera(const Vector3& camPos,
         const Vector3& targetPos,
         const Vector3& upVector,
         const float halfFOV,
         const float width,
         const float height,
         const float minZ,
         const float maxZ)
         : m_position(camPos),
           m_target(targetPos),
           m_up(upVector),
           m_halfFOV(halfFOV),
           m_screenWidth(width),
           m_screenHeight(height),
           m_near(minZ),
           m_far(maxZ) {}

  /**
  *  @brief Perpective camera constructor.
  *
  *  @param const Vector3& camPos
  *  @param const Vector3& targetPos
  *  @param const Vector3& upVector
  *  @param const float width
  *  @param const float height
  *  @param const float minZ
  *  @param const float maxZ
  */
  FORCEINLINE Camera(const Vector3& camPos,
         const Vector3& targetPos,
         const Vector3& upVector,
         const float width,
         const float height,
         const float minZ,
         const float maxZ)
         : m_position(camPos),
           m_target(targetPos),
           m_up(upVector),
           m_screenWidth(width),
           m_screenHeight(height),
           m_near(minZ),
           m_far(maxZ),
           m_bIsOrtho(true) {}

  /**
  *  @brief Default destructor.
  */
  ~Camera() = default;

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
  *  @brief Sets if camera is orthographic.
  *
  *  @param bool
  */
  FORCEINLINE void
  setIsOrthographic(const bool bOrtho);

  /**
  *  @brief Sets position.
  *
  *  @param Vector3&
  */
  FORCEINLINE void
  setPosition(const Vector3& position);

  /**
  *  @brief Sets the target.
  *
  *  @param Vector3&
  */
  FORCEINLINE void
  setTarget(const Vector3& target);

  /**
  *  @brief Sets the up vector.
  *
  *  @param Vector3&
  */
  FORCEINLINE void
  setUp(const Vector3& up);

  /**
  *  @brief Sets the width.
  *
  *  @param float
  */
  FORCEINLINE void
  setWidth(const float width);

  /**
  *  @brief Sets the height.
  *
  *  @param float
  */
  FORCEINLINE void
  setHeight(const float height);

  /**
  *  @brief Sets the half field of view.
  *
  *  @param float
  */
  FORCEINLINE void
  setHalfFOV(const float hFOV);

  /**
  *  @brief Sets the near component.
  *
  *  @param float
  */
  FORCEINLINE void
  setNear(const float near);

  /**
  *  @brief Sets the far component.
  *
  *  @param float
  */
  FORCEINLINE void
  setFar(const float far);

  /**
  *  @brief Gets the view matrix.
  *
  *  @return const ViewMatrix&
  */
  FORCEINLINE const Matrix4&
  getView() const;

  /**
  *  @brief Gets the projection matrix.
  *
  *  @return const ViewMatrix&
  */
  FORCEINLINE const Matrix4&
  getProjection() const;

  /**
  *  @brief Returns true if camera is orthographic.
  *
  *  @return bool
  */
  FORCEINLINE const bool
  isOrtho() const;

  /**
  *  @brief Gets the position.
  *
  *  @return Vector3&
  */
  FORCEINLINE const Vector3&
  getPosition() const;

  /**
  *  @brief Gets the target.
  *
  *  @return Vector3&
  */
  FORCEINLINE const Vector3&
  getTarget() const;

  /**
  *  @brief Gets the up vector.
  *
  *  @return Vector3&
  */
  FORCEINLINE const Vector3&
  getUp() const;

  /**
  *  @brief Gets the current half field of view.
  *
  *  @return float
  */
  FORCEINLINE const float
  getHalfFOV() const;

  /**
  *  @brief Gets the width.
  *
  *  @return float
  */
  FORCEINLINE const float
  getWidth() const;

  /**
  *  @brief Gets the height.
  *
  *  @return float
  */
  FORCEINLINE const float
  getHeight() const;

  /**
  *  @brief Gets the near component.
  *
  *  @return float
  */
  FORCEINLINE const float
  getNear() const;

  /**
  *  @brief Gets the far component.
  *
  *  @return float
  */
  FORCEINLINE const float
  getFar() const;

  /**
  *  @brief Gets the right vector of the camera.
  *
  *  @return Vector3
  */
  Vector3
  getRight();

  /**
  *  @brief Gets the foward vector of the camera.
  *
  *  @return Vector3
  */
  Vector3
  getFoward();

  /**
  *  @brief Update the camera.
  */
  void
  update();

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
  rotate(const float yaw, const float pitch);

  /**
  *  @brief Orbit camera around a point.
  *
  *  @param Radian& yaw
  *  @param Radian& pitch
  *  @param Vector3& center
  */
  void
  orbitCamera(const Radian& yaw, const Radian& pitch, const Vector3& center);

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 private:
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

  /**
  *  @brief Perspective near.
  */
  float m_near = 0.0f;

  /**
  *  @brief Perspective far.
  */
  float m_far = 0.0f;

  /**
  *  @brief Half field of view.
  */
  float m_halfFOV = 0.0f;

  /**
  *  @brief Screen width.
  */
  float m_screenWidth = 0.0f;

  /**
  *  @brief Screen height.
  */
  float m_screenHeight = 0.0f;

  /**
  *  @brief Is camera dirty?
  */
  bool m_bIsDirty = true;

  /**
  *  @brief Is camera orthographic.
  */
  bool m_bIsOrtho = false;

  /**
  *  @brief Camera frustum.
  */
  Frustum m_frustum = {};
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FORCEINLINE void
Camera::setIsOrthographic(const bool bOrtho)
{
  m_bIsOrtho = bOrtho;
}

FORCEINLINE void
Camera::setPosition(const Vector3& position)
{
  m_position = position;
}

FORCEINLINE void
Camera::setTarget(const Vector3& target)
{
  m_target = target;
}

FORCEINLINE void
Camera::setUp(const Vector3& up)
{
  m_up = up;
}

FORCEINLINE void
Camera::setWidth(const float width)
{
  m_screenWidth = width;
}

FORCEINLINE void
Camera::setHeight(const float height)
{
  m_screenHeight = height;
}

FORCEINLINE void
Camera::setHalfFOV(const float hFOV)
{
  m_halfFOV = hFOV;
}

FORCEINLINE void
Camera::setNear(const float near)
{
  m_near = near;
}

FORCEINLINE void
Camera::setFar(const float far)
{
  m_far = far;
}

FORCEINLINE const Matrix4&
Camera::getView() const
{
  return m_view;
}

FORCEINLINE const Matrix4&
Camera::getProjection() const
{
  return m_proj;
}

FORCEINLINE const bool
Camera::isOrtho() const
{
  return m_bIsOrtho;
}

FORCEINLINE const Vector3&
Camera::getPosition() const
{
  return m_position;
}

FORCEINLINE const Vector3&
Camera::getTarget() const
{
  return m_target;
}

FORCEINLINE const Vector3&
Camera::getUp() const
{
  return m_up;
}

FORCEINLINE const float
Camera::getHalfFOV() const
{
  return m_halfFOV;
}

FORCEINLINE const float
Camera::getWidth() const
{
  return m_screenWidth;
}

FORCEINLINE const float
Camera::getHeight() const
{
  return m_screenHeight;
}

FORCEINLINE const float
Camera::getNear() const
{
  return m_near;
}

FORCEINLINE const float
Camera::getFar() const
{
  return m_far;
}
}
