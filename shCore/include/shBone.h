/*************************************************************/
/*
*  @file    shBone.h
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
#include "shMatrix4.h"

namespace shEngineSDK {
struct SH_CORE_EXPORT Bone
{
  int32 id = 0;
  Matrix4 offset;
  String name;
  Vector<Bone> children;
};

struct SH_CORE_EXPORT Skeleton
{
  uint32 id = 0;
  uint32 boneCount = 0;
  Bone bones;
  Vector<Matrix4> currentPose;
};
}
