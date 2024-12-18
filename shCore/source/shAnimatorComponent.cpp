/*************************************************************/
/*
*  @file    shAnimatorComponent.cpp
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
#include "shAnimatorComponent.h"
#include "shMath.h"

namespace shEngineSDK {
void
AnimatorComponent::updateAnimation(const float elapsedTime)
{
  deltaTime = elapsedTime;
  
  if (currentAnim) {
    currentTime += currentAnim->ticksPerSecond * elapsedTime;
    currentTime = Math::fmod(currentTime, currentAnim->duration);
    calculateBoneTransform(currentAnim->skeletonData->bones, Matrix4::IDENTITY);
  }
}

void
AnimatorComponent::setCurrentAnimation(const SPtr<AnimationResource>& anim)
{
  currentAnim = anim;
  currentTime = 0.0f;
}

float
getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animTime)
{
  const float midWayLenght = animTime - lastTimeStamp;
  const float framesDiff = nextTimeStamp - lastTimeStamp;
  const float scaleFactor = midWayLenght / framesDiff;

  return scaleFactor;
}

uint32
getPositionIndex(const BoneTransformTrack* btt, float animTime) {
  for (uint32 index = 0; index < btt->numPositions - 1; ++index) {
    if (animTime < btt->positions[index + 1].timeStamp) {
      return index;
    }
  }
  return 0;
}

uint32
getRotationIndex(const BoneTransformTrack* btt, float animTime) {
  for (uint32 index = 0; index < btt->numRotations - 1; ++index) {
    if (animTime < btt->rotations[index + 1].timeStamp) {
      return index;
    }
  }
  return 0;
}

uint32
getScaleIndex(const BoneTransformTrack* btt, float animTime) {
  for (uint32 index = 0; index < btt->numScalings - 1; ++index) {
    if (animTime < btt->scales[index + 1].timeStamp) {
      return index;
    }
  }
  return 0;
}

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

void
AnimatorComponent::calculateBoneTransform(const Bone& bone, const Matrix4& parentTransform)
{
  String boneName = bone.name;
  Matrix4 boneTransform = bone.transformation;

  BoneTransformTrack* btt = nullptr;

  for (uint32 i = 0; i < currentAnim->boneTracks.size(); ++i) {
    if (currentAnim->boneTracks[i].name == boneName) {
      btt = &currentAnim->boneTracks[i];
    }
  }

  if (btt) {
    Matrix4 translation = interpolatePosition(btt, currentTime);
    Matrix4 rotation = interpolateRotation(btt, currentTime);
    Matrix4 scaling = interpolateScaling(btt, currentTime);
    boneTransform = translation * rotation * scaling;
  }

  const Matrix4 globalTransform = parentTransform * boneTransform;

  const uint32 index = bone.id;
  const Matrix4 offset = bone.offset;

  finalTransform[index] = globalTransform * offset;

  for (auto& child : bone.children) {
    calculateBoneTransform(child, globalTransform);
  }
}
}