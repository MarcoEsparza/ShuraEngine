/*************************************************************/
/*
*  @file    shAnimatorComponent.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/08
*  @brief   Component in charge of contain and play animations.
*
*  Component in charge of contain and play animations.
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
#include "shAnimatorComponent.h"
#include "shMath.h"

namespace shEngineSDK {
/*************************************************************/
/*
*  Non class functions
*/
/*************************************************************/

/**
*  @brief Gets normalized value for Lerp & Slerp.
* 
*  @param float lastTimeStamp
*  @param float nextTimeStamp
*  @param float animTime
*/
float
getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animTime)
{
  const float midWayLenght = animTime - lastTimeStamp;
  const float framesDiff = nextTimeStamp - lastTimeStamp;
  const float scaleFactor = midWayLenght / framesDiff;

  return scaleFactor;
}

/**
*  @brief Gets the current index on KeyPositions to interpolate to based on 
*         the current animation time.
* 
*  @param BoneTransformTrack* btt
*  @param float animTime
* 
*  @return uint32
*/
uint32
getPositionIndex(const BoneTransformTrack* btt, float animTime) {
  for (uint32 index = 0; index < btt->numPositions - 1; ++index) {
    if (animTime < btt->positions[index + 1].timeStamp) {
      return index;
    }
  }
  return 0;
}

/**
*  @brief Gets the current index on KeyRotations to interpolate to based on
*         the current animation time.
* 
*  @param BoneTransformTrack* btt
*  @param float animTime
* 
*  @return uint32
*/
uint32
getRotationIndex(const BoneTransformTrack* btt, float animTime) {
  for (uint32 index = 0; index < btt->numRotations - 1; ++index) {
    if (animTime < btt->rotations[index + 1].timeStamp) {
      return index;
    }
  }
  return 0;
}

/**
*  @brief Gets the current index on KeyScalings to interpolate to based on
*         the current animation time.
* 
*  @param BoneTransformTrack* btt
*  @param float animTime
* 
*  @return uint32
*/
uint32
getScaleIndex(const BoneTransformTrack* btt, float animTime) {
  for (uint32 index = 0; index < btt->numScalings - 1; ++index) {
    if (animTime < btt->scales[index + 1].timeStamp) {
      return index;
    }
  }
  return 0;
}

/**
*  @brief Figures out which position keys to interpolate b/w and performs the
*         interpolation and returns the translation matrix.
* 
*  @param BoneTransformTrack* btt
*  @param float animTime
* 
*  @return Matrix4
*/
Matrix4
interpolatePosition(const BoneTransformTrack* btt, float animTime)
{
  if (btt->numPositions == 1) {
    Matrix4 translation = Matrix4::IDENTITY;
    translation = translation.createTranslationMatrix(btt->positions[0].position);
    return translation;
  }
  else if (btt->numPositions == 0) {
    return Matrix4::IDENTITY;
  }

  const uint32 p0Index = getPositionIndex(btt, animTime);
  const uint32 p1Index = p0Index + 1;
  const float scaleFactor = getScaleFactor(btt->positions[p0Index].timeStamp,
                                           btt->positions[p1Index].timeStamp,
                                           animTime);
  const Vector3 finalPosition = btt->positions[p0Index].position.lerp(
                                btt->positions[p1Index].position,
                                scaleFactor);
  Matrix4 translation = Matrix4::IDENTITY;
  translation = translation.createTranslationMatrix(finalPosition);
  return translation;
}

/**
*  @brief Figures out which rotation keys to interpolate b/w and performs the
*         interpolation and returns the translation matrix.
* 
*  @param BoneTransformTrack* btt
*  @param float animTime
* 
*  @return Matrix4
*/
Matrix4
interpolateRotation(const BoneTransformTrack* btt, float animTime)
{
  if (btt->numRotations == 1) {
    Quaternion quat = btt->rotations[0].orientation;
    Matrix4 rotation(quat);
    return rotation;
  }
  else if (btt->numRotations == 0) {
    return Matrix4::IDENTITY;
  }

  const uint32 p0Index = getRotationIndex(btt, animTime);
  const uint32 p1Index = p0Index + 1;
  const float scaleFactor = getScaleFactor(btt->rotations[p0Index].timeStamp,
                                           btt->rotations[p1Index].timeStamp,
                                           animTime);
  const Quaternion finalRotation = btt->rotations[p0Index].orientation.slerp(
                                   btt->rotations[p1Index].orientation,
                                   scaleFactor);
  Matrix4 rotation(finalRotation);
  return rotation;
}

/**
*  @brief Figures out which scaling keys to interpolate b/w and performs the
*         interpolation and returns the translation matrix.
* 
*  @param BoneTransformTrack* btt
*  @param float animTime
* 
*  @return Matrix4
*/
Matrix4
interpolateScaling(const BoneTransformTrack* btt, float animTime)
{
  if (btt->numScalings == 1) {
    Matrix4 scale = Matrix4::IDENTITY;
    scale = scale.createScaleMatrix(btt->scales[0].scale);
    return scale;
  }
  else if (btt->numScalings == 0) {
    return Matrix4::IDENTITY;
  }

  const uint32 p0Index = getScaleIndex(btt, animTime);
  const uint32 p1Index = p0Index + 1;
  const float scaleFactor = getScaleFactor(btt->scales[p0Index].timeStamp,
                                           btt->scales[p1Index].timeStamp,
                                           animTime);
  const Vector3 finalScale = btt->scales[p0Index].scale.lerp(
                             btt->scales[p1Index].scale,
                             scaleFactor);
  Matrix4 scaling = Matrix4::IDENTITY;
  scaling = scaling.createScaleMatrix(finalScale);
  return scaling;
}

/*************************************************************/
/*
*  Animator functions
*/
/*************************************************************/

void
AnimatorComponent::updateAnimation(const float elapsedTime)
{
  deltaTime = elapsedTime;
  
  if (currentAnim) {
    currentTime += currentAnim->ticksPerSecond * elapsedTime;
    currentTime = Math::fmod(currentTime, currentAnim->duration);
    calculateBoneTransform(currentAnim->rootNode, Matrix4::IDENTITY);
  }
}

void
AnimatorComponent::setCurrentAnimation(const SPtr<AnimationResource>& anim)
{
  currentAnim = anim;
  currentTime = 0.0f;
  finalBoneTransforms.resize(currentAnim->skeletonData->boneCount);
}

void
AnimatorComponent::calculateBoneTransform(const AnimationNodeData& node, const Matrix4& parentTransform)
{
  String nodeName = node.name;
  Matrix4 nodeTransform = node.transformation;

  BoneTransformTrack* btt = currentAnim->findBone(nodeName);

  if (btt) {
    Matrix4 translation = interpolatePosition(btt, currentTime);
    Matrix4 rotation = interpolateRotation(btt, currentTime);
    Matrix4 scaling = interpolateScaling(btt, currentTime);
    btt->localTransform = translation * rotation * scaling;
    
    nodeTransform = btt->localTransform;
  }

  const Matrix4 globalTransform = parentTransform * nodeTransform;

  auto& boneInfoMap = currentAnim->skeletonData->boneInfoMap;

  if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
    int32 index = boneInfoMap[nodeName].id;
    Matrix4 offset = boneInfoMap[nodeName].offset;
    finalBoneTransforms[index] = globalTransform * offset;
  }

  for (auto& child : node.children) {
    calculateBoneTransform(child, globalTransform);
  }
}
}