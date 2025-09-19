/*****************************************************************************/
/*
*  @file    shAnimatorComponent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/09/15
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
class ConstantBuffer;

struct BoneTransformCB
{
  Matrix4 finalTransforms[100];
};

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
  readNodeHierarchy(float time,
                    const SPtr<BoneHierarchy> node,
                    const Matrix4& parentTransform);

  Matrix4
  composeTransform(const BoneAnimationChannel& channel, float time);

  Vector3
  getInterpolatedPosition(const BoneAnimationChannel& channel, float time);

  Quaternion
  getInterpolatedRotation(const BoneAnimationChannel& channel, float time);

  Vector3
  getInterpolatedScale(const BoneAnimationChannel& channel, float time);

  void
  updateCB();

  template<typename T>
  int32
  getKeyIndex(const Vector<T>& keys, float time)
  {
    if(keys.empty()) {
      return -1;
    }
    if(keys.size() == 1) {
      return 0;
    }
    int32 index = 0;
    for(int32 i = cast::st<int32>(keys.size()) - 2; i >= 0; --i) {
      if(time >= keys[i].timeStamp) {
        index = i;
        break;
      }
    }
    return index;
  }

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
  *  @brief Final skeleton transformation.
  */
  Vector<Matrix4> m_finalBoneTransforms;

  BoneTransformCB m_boneTransformData;

  SPtr<ConstantBuffer> m_boneTransformCB;
};
}