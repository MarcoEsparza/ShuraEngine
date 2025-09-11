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
  void
  update(float deltaTime);

 private:
  void
  ReadNodeHierarchy(float time, const Matrix4& parentTransform);

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
  *  @brief Pointer to the animation skeleton.
  */
  SPtr<SkeletonResource> m_skeletonData;

  /**
  *  @brief The current time for the animation.
  */
  float m_currentTime = 0.0f;

  /**
  *  @brief Final skeleton trsnformation.
  */
  Vector<Matrix4> m_finalBoneTransforms;
};
}