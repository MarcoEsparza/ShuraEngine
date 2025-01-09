/*************************************************************/
/*
*  @file    shAnimationResource.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/08
*  @brief   Animation resource, it cointains all info for one animation,
*           it also have a pointer to the skeleton.
*
*  Animation resource, it cointains all info for one animation,
*  it also have a pointer to the skeleton.
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
#include "shSkeletonResource.h"
#include "shVector3.h"
#include "shVector4.h"
#include "shQuaternion.h"
#include "shMatrix4.h"

namespace shEngineSDK {
/**
*  @brief Struct for animation position keys.
*/
struct SH_CORE_EXPORT KeyPosition
{
  /**
  *  @brief Key position.
  */
  Vector3 position = Vector3(0.0f, 0.0f, 0.0f);

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
  Quaternion orientation = Quaternion(0.0f, 0.0f, 0.0f,  0.0f);

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
  Vector3 scale = Vector3(0.0f, 0.0f, 0.0f);

  /**
  *  @brief Key time.
  */
  float timeStamp = 0.0f;
};

/**
*  @brief Struct for animation bone transforms.
*/
struct SH_CORE_EXPORT BoneTransformTrack
{
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

  /**
  *  @brief Number of bone position keys.
  */
  uint32 numPositions = 0;

  /**
  *  @brief Number of bone rotation keys.
  */
  uint32 numRotations = 0;

  /**
  *  @brief Number of bone scale keys.
  */
  uint32 numScalings = 0;

  /**
  *  @brief Bone local transform.
  */
  Matrix4 localTransform = Matrix4::IDENTITY;

  /**
  *  @brief Bone name.
  */
  String name = "";

  /**
  *  @brief Bone ID.
  */
  int32 ID = 0;
};

/**
*  @brief Struct for the file animation nodes.
*/
struct AnimationNodeData
{
  /**
  *  @brief Node transform.
  */
  Matrix4 transformation = Matrix4::IDENTITY;

  /**
  *  @brief Bone name.
  */
  String name;

  /**
  *  @brief Number of children.
  */
  uint32 childrenCount = 0;

  /**
  *  @brief Node children.
  */
  Vector<AnimationNodeData> children;
};

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

  /*************************************************************/
  /*
  *  Functtions
  */
  /*************************************************************/
 public:
  /**
  *  @brief 
  */
  FORCEINLINE BoneTransformTrack*
  findBone(const String& name);

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /**
  *  @brief Animation can loop?
  */
  bool hasLoop = false;

  /**
  *  @brief Animation duration in ticks.
  */
  float duration = 0.0f;

  /**
  *  @brief How many ticks per second.
  */
  float ticksPerSecond = 0.0f;

  /**
  *  @brief Bone transform tracks.
  */
  Vector<BoneTransformTrack> boneTracks;

  /**
  *  @brief Node Data.
  */
  AnimationNodeData rootNode;

  /**
  *  @brief Pointer to the animation skeleton.
  */
  SPtr<SkeletonResource> skeletonData;
};

FORCEINLINE BoneTransformTrack*
AnimationResource::findBone(const String& name)
{
  Vector<BoneTransformTrack>::iterator iter = std::find_if(boneTracks.begin(), boneTracks.end(),
    [&](const BoneTransformTrack& btt)
    {
      return btt.name == name;
    }
  );

  if (iter == boneTracks.end()) {
    return nullptr;
  }
  else {
    return &(*iter);
  }
}
}
