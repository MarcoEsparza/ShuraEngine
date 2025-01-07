/*************************************************************/
/*
*  @file    shSkeletonResource.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/15
*  @brief   Skeleton info class.
*
*  Skeleton info class.
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
/**
*  @brief Bone base structure.
*/
struct SH_CORE_EXPORT Bone
{
  /**
  *  @brief ID.
  */
  int32 id = 0;

  /**
  *  @brief Offset matrix for trnsformation.
  */
  Matrix4 offset;

  /**
  *  @brief Base transform.
  */
  Matrix4 transformation;

  /**
  *  @brief Bone name.
  */
  String name;

  /**
  *  @brief All bone children.
  */
  Vector<Bone> children;
};

struct BoneInfo
{
  int32 id;
  Matrix4 offset;
  String name;
};
  
/**
*  @brief Skeleton info class.
*/
class SkeletonResource : public Resource
{
 public:
  /**
  *  @brief Default constructor.
  */
  SkeletonResource() : Resource(RESOURCE_TYPE::kSkeleton) {}

  /**
  *  @brief Default destructor.
  */
  ~SkeletonResource() = default;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/

  /**
  *  @brief Skeleton id.
  */
  uint32 id = 0;

  /**
  *  @brief Number of bones.
  */
  uint32 boneCount = 0;

  /**
  *  @brief Root bone.
  */
  Bone bones;

  /**
  *  @brief Inverse transform for the skeleton.
  */
  Matrix4 inverseTransform;

  /**
  *  @brief Map for bone offsets.
  */
  UMap<String, std::pair<int32, Matrix4>> boneInfo;

  UMap<String, BoneInfo> boneInfoMap;
};
}
