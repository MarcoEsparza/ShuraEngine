/*****************************************************************************/
/*
*  @file    shCamera.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/30
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
  Camera(const Vector3& camPos,
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
  *  @brief
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
  float m_near;

  /**
  *  @brief Perspective far.
  */
  float m_far;

  /**
  *  @brief Half field of view.
  */
  float m_halfFOV;

  /**
  *  @brief Screen width.
  */
  float m_screenWidth;

  /**
  *  @brief Screen height.
  */
  float m_screenHeight;

  /**
  *  @brief 
  */
  bool m_bIsDirty = true;

  /**
  *  @brief
  */
  bool m_bIsOrtho = false;

  /**
  *  @brief
  */
  Frustum m_frustum;
};
}
