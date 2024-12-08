/*************************************************************/
/*
*  @file    shAnimationResource.h
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
#include "shResource.h"
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

class AnimationResource : public Resource
{
 public:
  AnimationResource() { type = RESOURCE_TYPE::kAnmimation; }
  ~AnimationResource() = default;

  bool hasLoop = false;
  float duration = 0.0f;
  float ticksPerSecond = 0.0f;
  UMap<String, BoneTransformTrack> boneTransform;
};
}
