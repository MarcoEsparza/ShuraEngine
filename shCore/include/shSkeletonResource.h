/*****************************************************************************/
/*
*  @file    shSkeletonResource.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/09/10
*  @brief   Skeleton info class.
*
*  Skeleton info class.
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
#include "shResource.h"
#include <shMatrix4.h>

namespace shEngineSDK {
/**
*  @brief Struct for the bone info.
*/
struct SH_CORE_EXPORT ALIGN_AS(16) BoneInfo
{
  /**
  *  @brief Bone name.
  */
  String name;

  /**
  *  @brief Bone offset.
  */
  Matrix4 offset = Matrix4::IDENTITY;

  /**
  *  @brief Bone offset.
  */
  //Matrix4 finalTransformation = Matrix4::IDENTITY;
};

struct SH_CORE_EXPORT BoneHierarchy
{
  String name;
  Matrix4 transformation = Matrix4::IDENTITY;
  Vector <SPtr<BoneHierarchy>> children;
};

/**
*  @brief Skeleton info class.
*/
class SH_CORE_EXPORT SkeletonResource : public Resource
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

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  int32
  getBoneID(const String& name) const;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Number of bones.
  */
  uint32 m_boneCount = 0;

  /**
  *  @brief Map with all bones.
  */
  UMap<String, int32> m_boneMapping;

  SPtr<BoneHierarchy> m_rootBone;

  /**
  *  @brief Bone container.
  */
  Vector<BoneInfo> m_bones;
};
}
