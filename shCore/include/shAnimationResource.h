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
#include "shQuaternion.h"
#include "shMatrix4.h"
#include "shSkeletonResource.h"

namespace shEngineSDK {
struct SH_CORE_EXPORT KeyPosition
{
  Vector3 position;
  float timeStamp;
};

struct SH_CORE_EXPORT KeyRotation
{
  Quaternion orientation;
  float timeStamp;
};

struct SH_CORE_EXPORT KeyScale
{
  Vector3 scale;
  float timeStamp;
};

struct SH_CORE_EXPORT BoneTransformTrack
{
  Vector<KeyPosition> positions;
  Vector<KeyRotation> rotations;
  Vector<KeyScale> scales;
  uint32 numPositions;
  uint32 numRotations;
  uint32 numScalings;

  Matrix4 localTransform;
  String name;
  int32 ID;
};

class AnimationResource : public Resource
{
 public:
  AnimationResource() { type = RESOURCE_TYPE::kAnmimation; }
  ~AnimationResource() = default;

  bool hasLoop = false;
  float duration = 0.0f;
  float ticksPerSecond = 0.0f;
  Vector<BoneTransformTrack> boneTTracks;
  SPtr<SkeletonResource> skeletonData;
};
}
