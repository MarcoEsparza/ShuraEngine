/*************************************************************/
/*
*  @file    shSkeletonResource.h
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
#include "shMatrix4.h"

namespace shEngineSDK {
struct SH_CORE_EXPORT Bone
{
  int32 id = 0;
  Matrix4 offset;
  String name;
  Vector<Bone> children;
};
  
/**
*  @brief 
*/
class SkeletonResource : public Resource
{
 public:
  SkeletonResource() { type = RESOURCE_TYPE::kSkeleton; }
  ~SkeletonResource() = default;

  uint32 id = 0;
  uint32 boneCount = 0;
  Bone bones;
  Matrix4 inverseTransform;
  UMap<String, std::pair<int32, Matrix4>> boneInfo;
};
}
