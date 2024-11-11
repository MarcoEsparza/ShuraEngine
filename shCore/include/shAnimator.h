/*************************************************************/
/*
*  @file    shAnimator.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/06
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
#include "shBone.h"
#include "shVector3.h"
#include "shVector4.h"

namespace shEngineSDK {
struct SH_CORE_EXPORT BoneTransformTrack
{
  Vector<float> posTimestamps;
  Vector<float> rotTimestamps;
  Vector<float> scaleTimestamps;

  Vector<Vector3> positions;
  Vector<Vector4> rotations;
  Vector<Vector3> scales;
};

struct SH_CORE_EXPORT Animation
{
  bool hasLoop;
  float duration;
  float ticksPerSecond;
  UMap<String, BoneTransformTrack> boneTransform;
};

class SH_CORE_EXPORT Animator
{
 public:
  Animator() = default;
  ~Animator() = default;

  /*void
  getPose(Animation& anim, Bone& sBone, Vector<Matrix4>& pose, float dt);

  Pair<uint32, float>
  getTimeFraction(const Vector<float>& times, float dt);*/
};
}
