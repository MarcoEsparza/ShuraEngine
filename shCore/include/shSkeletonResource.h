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
//struct SH_CORE_EXPORT Bone
//{
//  /**
//  *  @brief ID.
//  */
//  int32 id = 0;
//
//  /**
//  *  @brief Offset matrix for trnsformation.
//  */
//  Matrix4 offset;
//
//  /**
//  *  @brief Base transform.
//  */
//  Matrix4 transformation;
//
//  /**
//  *  @brief Bone name.
//  */
//  String name;
//
//  /**
//  *  @brief All bone children.
//  */
//  Vector<Bone> children;
//};

struct BoneInfo
{
  /**
  *  @brief Bone ID.
  */
  int32 id;

  /**
  *  @brief Bone offset.
  */
  Matrix4 offset;

  /**
  *  @brief Bone name.
  */
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
  *  @brief Number of bones.
  */
  uint32 boneCount = 0;

  /**
  *  @brief Map with all bones.
  */
  UMap<String, BoneInfo> boneInfoMap;
};
}
