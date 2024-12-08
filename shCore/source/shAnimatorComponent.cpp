/*************************************************************/
/*
*  @file    shAnimatorComponent.cpp
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
#include "shAnimatorComponent.h"
#include "shMath.h"

namespace shEngineSDK {
uint32
getCurrentIndex(const Vector<float>& times, float dt)
{
  if (times.size() < 1) {
    return 0;
  }

  for (uint32 i = 0; i < times.size(); ++i) {
    float t = times[i];
    if (dt < t) {
      return i;
    }
  }

  return 0;
}

void
AnimatorComponent::getPose(Bone& sBone,
                           Vector<Matrix4>& pose,
                           Matrix4 parentTransform,
                           Matrix4 globalInvTransform,
                           uint32 animIndex,
                           float timeInSeconds)
{
  SPtr<AnimationResource>& currentAnim = animations[animIndex];

  if (currentAnim->boneTransform.find(sBone.name) != currentAnim->boneTransform.end()) {
    BoneTransformTrack& btt = currentAnim->boneTransform[sBone.name];

    //float timeInTicks = timeInSeconds * currentAnim->ticksPerSecond;
    //float animationTimeTicks = Math::fmod(timeInTicks, currentAnim->duration);
    uint32 posTime = getCurrentIndex(btt.posTimestamps, timeInSeconds);
    int32 prevPosTime = posTime - 1;

    if (prevPosTime < 0) {
      prevPosTime = 0;
    }

    const Vector3 pos1 = btt.positions[prevPosTime];
    const Vector3 pos2 = btt.positions[posTime];

    const Vector3 pos = pos1.lerp(pos2, timeInSeconds);

    uint32 rotTime = getCurrentIndex(btt.rotTimestamps, timeInSeconds);
    int32 prevRotTime = rotTime - 1;

    if (prevRotTime < 0) {
      prevRotTime = 0;
    }

    const Quaternion rot1(btt.rotations[prevRotTime].w,
                          btt.rotations[prevRotTime].x,
                          btt.rotations[prevRotTime].y,
                          btt.rotations[prevRotTime].z);

    const Quaternion rot2(btt.rotations[posTime].w,
                          btt.rotations[posTime].x,
                          btt.rotations[posTime].y,
                          btt.rotations[posTime].z);

    const Quaternion rot = rot1.lerp(rot2, timeInSeconds);

    uint32 scaleTime = getCurrentIndex(btt.scaleTimestamps, timeInSeconds);
    int32 prevScaleTime = scaleTime - 1;

    if (prevScaleTime < 0) {
      prevScaleTime = 0;
    }

    const Vector3 scale1 = btt.scales[prevScaleTime];
    const Vector3 scale2 = btt.scales[posTime];

    const Vector3 scale = scale1.lerp(scale2, timeInSeconds);

    Matrix4 temp = Matrix4::IDENTITY;
    Matrix4 posMat = temp.createTranslationMatrix(pos);
    Matrix4 rotMat(rot);
    Matrix4 scaleMat = temp.createScaleMatrix(scale);

    Matrix4 localTransform = posMat * rotMat * scaleMat;
    Matrix4 globalTransform = parentTransform * localTransform;

    pose[sBone.id] = globalInvTransform * globalTransform * sBone.offset;

    for (Bone& child : sBone.children) {
      getPose(child, pose, globalTransform, globalInvTransform, animIndex, timeInSeconds);
    }
  }

  for (Bone& child : sBone.children) {
    getPose(child, pose, parentTransform, globalInvTransform, animIndex, timeInSeconds);
  }
}
}