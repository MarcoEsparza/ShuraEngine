/*************************************************************/
/*
*  @file    shAnimatorComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/04
*  @brief
*
*
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
#include "shAnimationResource.h"
#include "shSkeletonResource.h"
#include "shMatrix4.h"

namespace shEngineSDK {
class AnimatorComponent : public Component
{
 public:
  AnimatorComponent() { type = COMPONENT_TYPE::kAnimator; }
  ~AnimatorComponent() = default;

  void
  getPose(Bone& sBone,
          Vector<Matrix4>& pose,
          Matrix4 parentTransform,
          Matrix4 globalInvTransform,
          uint32 animIndex,
          float timeInSeconds);

  Vector<SPtr<AnimationResource>> animations;
};
}