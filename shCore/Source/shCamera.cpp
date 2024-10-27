/*************************************************************/
/*
*  @file    shCamera.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/26
*  @brief   Engine Camera class.
*
*  Engine Camera class.
*
*  @bug     RotateCam function not working properly.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shCamera.h"
#include "shMath.h"

namespace shEngineSDK {
void
Camera::setViewData(const Vector3& camPos,
                    const Vector3& targetPos,
                    const Vector3& upVector)
{
  m_view = ViewMatrix(camPos, targetPos, upVector);

  m_position = camPos;
  m_target = targetPos;
  m_upVector = upVector;
}

void
Camera::setProjectionData(const float halfFOV,
                          const float width,
                          const float height,
                          const float minZ,
                          const float maxZ)
{
  m_proj = ProjectionMatrix(halfFOV, width, height, minZ, maxZ);

  m_halfFOV = halfFOV;
  m_width = width;
  m_height = height;
  m_minZ = minZ;
  m_maxZ = maxZ;
}

void
FPSCamera::moveX(const float dir)
{
  Vector3 forward = m_target - m_position;
  m_right = m_upVector.cross(forward);
  m_right.normalize();

  m_position += m_right * dir;
  m_target += m_right * dir;

  setViewData(m_position, m_target, m_upVector);
}

void
FPSCamera::moveY(const float dir)
{
  Vector3 forward = m_target - m_position;
  m_upVector = forward.cross(m_right);
  m_upVector.normalize();

  m_position += m_upVector * dir;
  m_target += m_upVector * dir;

  setViewData(m_position, m_target, m_upVector);
}

void
FPSCamera::moveZ(const float dir)
{
  Vector3 forward = m_right.cross(m_upVector);
  forward.normalize();

  m_position += forward * dir;
  m_target += forward * dir;

  setViewData(m_position, m_target, m_upVector);
}

// TODO: This function dont work properly, it will be fixed later.
void
FPSCamera::rotateCam(const float yaw, const float pitch)
{
  Vector3 forward = m_target - m_position;
  forward.normalize();

  Vector3 right = m_upVector.cross(forward);
  right.normalize();

  Vector3 up = forward.cross(right);
  up.normalize();

  Matrix4 rotation = Matrix4::identity;
  Matrix4 rotX = rotation.createRotationXMatrix(pitch);
  Matrix4 rotY = rotation.createRotationYMatrix(yaw);
  rotation = rotX * rotY;

  Vector3 vecZ = rotation.transformDirection(forward);
  Vector3 vecX = rotation.transformDirection(right);
  Vector3 vecY = rotation.transformDirection(up);

  forward += vecX.cross(vecY);

  m_target = m_position + forward;

  setViewData(m_position, m_target, m_upVector);
}
}
