/*****************************************************************************/
/*
*  @file    shAnimatorComponent.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/09/10
*  @brief   Component in charge of contain and play animations.
*
*  Component in charge of contain and play animations.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shAnimatorComponent.h"
#include <shMath.h>

namespace shEngineSDK {
void
AnimatorComponent::update(float deltaTime)
{
  if(m_currentAnim == nullptr || m_skeletonData == nullptr) {
    return;
  }

  float tps = (m_currentAnim->m_ticksPerSecond != 0.0f) ?
               m_currentAnim->m_ticksPerSecond : 25.0f;
  m_currentTime += tps * deltaTime;
  m_currentTime = Math::fmod(m_currentTime, m_currentAnim->m_duration);


}

void
AnimatorComponent::ReadNodeHierarchy(float time, const Matrix4& parentTransform)
{
  
}
}
