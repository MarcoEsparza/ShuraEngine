/*****************************************************************************/
/*
*  @file    shCamera.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/15
*  @brief   Engine Camera class.
*
*  Engine Camera class.
*
*  @bug     Camera movement not working properly.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
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
  m_up = upVector;
}

void
Camera::setPerspectiveData(const float halfFOV,
                          const float width,
                          const float height,
                          const float minZ,
                          const float maxZ)
{
  m_proj = ProjectionMatrix(halfFOV, width, height, minZ, maxZ);

  m_halfFOV = halfFOV;
  m_screenWidth = width;
  m_screenHeight = height;
  m_near = minZ;
  m_far = maxZ;
}

void
Camera::setOrthographicProjData(const float left,
                                const float right,
                                const float bottom,
                                const float top,
                                const float nearZ,
                                const float farZ)
{
  m_proj = OrthographicProjectionMatrix(left, right, bottom, top, nearZ, farZ);

  m_near = nearZ;
  m_far = farZ;
  m_bIsOrtho = true;
}

Vector3
Camera::getRight()
{
  return Vector3::UP.cross(getFoward()).getNormalized();
}

Vector3
Camera::getFoward()
{
  return (m_target - m_position);
}

void
Camera::update()
{
  Vector3 dir = (m_target - m_position).getNormalized();
  float DdU = Math::abs(dir.dot(Vector3::UP));

  if (DdU > 0.999f) {
    m_up = Vector3::RIGHT;
  }
  else {
    m_up = Vector3::UP;
  }

  m_view = ViewMatrix(m_position, m_target, m_up);
  
  if (m_bIsOrtho) {
    m_proj = OrthographicProjectionMatrix(-m_screenWidth * 0.5f,
                                          m_screenWidth * 0.5f,
                                          -m_screenHeight * 0.5f,
                                          m_screenHeight * 0.5f,
                                          m_near,
                                          m_far);
  }
  else {
    m_proj = ProjectionMatrix(m_halfFOV,
                              m_screenWidth,
                              m_screenHeight,
                              m_near,
                              m_far);
  }
  
  m_frustum.calculatePlanes(m_view, m_proj);
  
  m_bIsDirty = false;
}

void
Camera::move(const Vector3& direction)
{
  Vector3 offset = (getRight() * direction.x) +
                   (Vector3::UP * direction.y) +
                   (getFoward() * direction.z);
  m_position += offset;
  m_target += offset;

  m_bIsDirty = true;
}

void
Camera::rotate(const float yaw, const float pitch)
{
  // Clamp pitch to prevent gimbal lock
  //const float limit = 89.0f * Math::DEG2RAD; // 89 degrees in radians
  //pitch = Math::clamp(pitch, -limit, limit);

  Matrix4 rotation = MatrixRotationAxis(getRight(), pitch) *
                     MatrixRotationAxis(Vector3::UP, yaw);

  Vector3 newFoward = (rotation * getFoward()).getNormalized();

  m_target = m_position + newFoward;

  m_bIsDirty = true;
}

void
Camera::orbitCamera(const Radian& yaw, const Radian& pitch, const Vector3& center)
{
  Vector3 forward = m_position - center;

  Vector3 forwardNormal = forward.getNormalized();
  float dot = forwardNormal.dot(Vector3::UP);
  Radian currentPitch;
  currentPitch = Math::acos(Radian(dot));

  float newPitch = currentPitch.getValueOnRadians() + pitch.getValueOnRadians();
  newPitch = Math::clamp(newPitch, Degree(5.0f).getValueOnRadians(), Degree(174.0f).getValueOnRadians());

  Quaternion yawRot(Vector3::UP, yaw.getValueOnRadians());
  Quaternion pitchRot(getRight(), newPitch - currentPitch.getValueOnRadians());

  forward = yawRot * forward;
  Vector3 tempForward = forward;
  forward = pitchRot * forward;

  dot = forward.getNormalized().dot(Vector3::UP);
  if (Math::abs(dot) > 0.99f) {
    forward = tempForward;
  }

  //m_position = center + forward;
  //m_target = center;

  setViewData(center + forward, center, Vector3::UP);

  m_bIsDirty = true;
}
}
