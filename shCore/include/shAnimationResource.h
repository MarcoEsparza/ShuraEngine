/*****************************************************************************/
/*
*  @file    shAnimationResource.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/09/10
*  @brief   Animation resource, it cointains all info for one animation,
*           it also have a pointer to the skeleton.
*
*  Animation resource, it cointains all info for one animation,
*  it also have a pointer to the skeleton.
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
#include "shSkeletonResource.h"
#include <shVector3.h>
#include <shVector4.h>
#include <shQuaternion.h>
#include <shMatrix4.h>

namespace shEngineSDK {
/**
*  @brief Struct for animation position keys.
*/
struct SH_CORE_EXPORT KeyPosition
{
  /**
  *  @brief Key position.
  */
  Vector3 position = Vector3::ZERO;

  /**
  *  @brief Key time.
  */
  float timeStamp = 0.0f;
};

/**
*  @brief Struct for animation rotation keys.
*/
struct SH_CORE_EXPORT KeyRotation
{
  /**
  *  @brief Key rotation.
  */
  Quaternion orientation = Quaternion::ZERO;

  /**
  *  @brief Key time.
  */
  float timeStamp = 0.0f;
};

/**
*  @brief Struct for animation scale keys.
*/
struct SH_CORE_EXPORT KeyScale
{
  /**
  *  @brief Key scale.
  */
  Vector3 scale = Vector3::ZERO;

  /**
  *  @brief Key time.
  */
  float timeStamp = 0.0f;
};

/**
*  @brief Struct for animation bone transforms.
*/
struct SH_CORE_EXPORT ALIGN_AS(16) BoneAnimationChannel
{
  /**
  *  @brief Bone name.
  */
  String name = "";

  /**
  *  @brief Bone position keys.
  */
  Vector<KeyPosition> positions;

  /**
  *  @brief Bone rotation keys.
  */
  Vector<KeyRotation> rotations;

  /**
  *  @brief Bone scale keys.
  */
  Vector<KeyScale> scales;
};

/**
*  @brief Struct for the file animation nodes.
*/
//struct AnimationNodeData
//{
//  /**
//  *  @brief Node transform.
//  */
//  Matrix4 transformation = Matrix4::IDENTITY;
//
//  /**
//  *  @brief Bone name.
//  */
//  String name;
//
//  /**
//  *  @brief Number of children.
//  */
//  uint32 childrenCount = 0;
//
//  /**
//  *  @brief Node children.
//  */
//  Vector<AnimationNodeData> children;
//};

/**
*  @brief Animation resource, it cointains all info for one animation,
*         it also have a pointer to the skeleton.
*/
class AnimationResource : public Resource
{
 public:
  /**
  *  @brief Default constructor.
  */
  AnimationResource() : Resource(RESOURCE_TYPE::kAnmimation) {}

  /**
  *  @brief Default destructor.
  */
  ~AnimationResource() = default;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Animation duration in ticks.
  */
  float m_duration = 0.0f;

  /**
  *  @brief How many ticks per second.
  */
  float m_ticksPerSecond = 0.0f;

  /**
  *  @brief Animation channels.
  */
  UMap<String, BoneAnimationChannel> m_channels;

  /**
  *  @brief Pointer to the animation skeleton.
  */
  WPtr<SkeletonResource> m_skeletonData;
};
}
