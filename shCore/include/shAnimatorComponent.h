/*****************************************************************************/
/*
*  @file    shAnimatorComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/16
*  @brief   Component in charge of contain and play animations.
*
*  Component in charge of contain and play animations.
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
#include "shAnimationResource.h"
#include "shSkeletonResource.h"
#include <shMatrix4.h>

namespace shEngineSDK {
/**
*  @brief Component in charge of contain and play animations.
*/
class AnimatorComponent : public Component
{
 public:
  /**
  *  @brief Default constructor.
  */
  AnimatorComponent() : Component(COMPONENT_TYPE::kAnimator) {}

  /**
  *  @brief Default destructor.
  */
  ~AnimatorComponent() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Updates the current animation.
  * 
  *  @param float elapsedTime
  */
  void
  updateAnimation(const float elapsedTime);

  /**
  *  @brief Select an animation to play.
  * 
  *  @param SPtr<AnimationResource>& anim
  */
  void
  setCurrentAnimation(const SPtr<AnimationResource>& anim);

 private:
  /**
  *  @brief Calculates all bones transformations throgh the time.
  * 
  *  @param Bone& bone
  *  @param Matrix4 parentTransform
  */
  void
  calculateBoneTransform(const AnimationNodeData& node, const Matrix4& parentTransform);

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief The current animation.
  */
  SPtr<AnimationResource> m_currentAnim;

  /**
  *  @brief The current time for the animation.
  */
  float m_currentTime = 0.0f;

  /**
  *  @brief The time for the animator.
  */
  float m_deltaTime = 0.0f;

  /**
  *  @brief Final skeleton trsnformation.
  */
  Vector<Matrix4> m_finalBoneTransforms;
};
}