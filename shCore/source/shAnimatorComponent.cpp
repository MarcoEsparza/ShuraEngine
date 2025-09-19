/*****************************************************************************/
/*
*  @file    shAnimatorComponent.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/09/15
*  @brief   Component in charge of contain and play animations.
*
*  Component in charge of contain and play animations.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shAnimatorComponent.h"
#include "shGraphicsManager.h"
#include <shMath.h>

namespace shEngineSDK {
void
AnimatorComponent::update(float deltaTime)
{
  if(m_currentAnim == nullptr || m_skeletonData == nullptr) {
    return;
  }

  float tps = (m_currentAnim->m_ticksPerSecond != 0.0f) ?
               m_currentAnim->m_ticksPerSecond : 25.0f;
  m_currentTime += tps * deltaTime;
  m_currentTime = Math::fmod(m_currentTime, m_currentAnim->m_duration);

  readNodeHierarchy(m_currentTime, m_skeletonData->m_rootBone, Matrix4::IDENTITY);
  updateCB();
}

void
AnimatorComponent::readNodeHierarchy(float time,
                                     const SPtr<BoneHierarchy> node,
                                     const Matrix4& parentTransform)
{
  String nodeName = node->name;
  Matrix4 nodeTransform = node->transformation;

  auto channelIt = m_currentAnim->m_channels.find(nodeName);
  if(channelIt != m_currentAnim->m_channels.end()) {
    const BoneAnimationChannel& channel = channelIt->second;
    nodeTransform = composeTransform(channel, time);
  }

  Matrix4 globalTransform = nodeTransform * parentTransform;

  int32 boneID = m_skeletonData->getBoneID(nodeName);
  if(boneID >= 0) {
    m_finalBoneTransforms[boneID] =
      m_skeletonData->m_bones[boneID].offset * globalTransform;
  }

  for (uint32 i = 0; i < node->children.size(); ++i) {
    readNodeHierarchy(time, node->children[i], globalTransform);
  }
}

Matrix4
AnimatorComponent::composeTransform(const BoneAnimationChannel& channel, float time)
{
  Vector3 position = getInterpolatedPosition(channel, time);
  Quaternion rotation = getInterpolatedRotation(channel, time);
  Vector3 scale = getInterpolatedScale(channel, time);

  Matrix4 translationMat = Matrix4().createTranslationMatrix(position);
  Matrix4 rotationMat = Matrix4(rotation);
  Matrix4 scaleMat = Matrix4().createScaleMatrix(scale);

  return scaleMat * rotationMat * translationMat;
}

Vector3
AnimatorComponent::getInterpolatedPosition(const BoneAnimationChannel& channel, float time)
{
  if(channel.positions.empty()) {
    return Vector3::ZERO;
  }
  if(channel.positions.size() == 1) {
    return channel.positions[0].position;
  }

  /*int32 index = 0;
  for(int32 i = cast::st<int32>(channel.positions.size()) - 2; i >= 0; --i) {
    if(time >= channel.positions[i].timeStamp) {
      index = i;
      break;
    }
  }*/

  int32 index = getKeyIndex(channel.positions, time);
  int32 nextIndex = index + 1;
  float deltaTime = channel.positions[nextIndex].timeStamp -
                    channel.positions[index].timeStamp;
  float factor = (time - channel.positions[index].timeStamp) / deltaTime;

  const Vector3& start = channel.positions[index].position;
  const Vector3& end = channel.positions[nextIndex].position;

  return start.lerp(end, factor);
}

Quaternion
AnimatorComponent::getInterpolatedRotation(const BoneAnimationChannel& channel, float time)
{
  if(channel.rotations.empty()) {
    return Quaternion::IDENTITY;
  }
  if(channel.rotations.size() == 1) {
    return channel.rotations[0].orientation;
  }

  /*int32 index = 0;
  for (int32 i = cast::st<int32>(channel.rotations.size()) - 2; i >= 0; --i) {
    if (time >= channel.rotations[i].timeStamp) {
      index = i;
      break;
    }
  }*/

  int32 index = getKeyIndex(channel.rotations, time);
  int32 nextIndex = index + 1;
  float deltaTime = channel.rotations[nextIndex].timeStamp -
                    channel.rotations[index].timeStamp;
  float factor = (time - channel.rotations[index].timeStamp) / deltaTime;

  return channel.rotations[index].orientation.slerp(
         channel.rotations[nextIndex].orientation, factor);
}

Vector3
AnimatorComponent::getInterpolatedScale(const BoneAnimationChannel& channel, float time)
{
  if(channel.scales.empty()) {
    return Vector3::ONE;
  }
  if(channel.scales.size() == 1) {
    return channel.scales[0].scale;
  }

  /*int32 index = 0;
  for (int32 i = cast::st<int32>(channel.scales.size()) - 2; i >= 0; --i) {
    if (time >= channel.scales[i].timeStamp) {
      index = i;
      break;
    }
  }*/

  int32 index = getKeyIndex(channel.scales, time);
  int32 nextIndex = index + 1;
  float deltaTime = channel.scales[nextIndex].timeStamp -
                    channel.scales[index].timeStamp;
  float factor = (time - channel.scales[index].timeStamp) / deltaTime;

  Vector3 start = channel.scales[index].scale;
  Vector3 end = channel.scales[nextIndex].scale;

  return start.lerp(end, factor);
}

void
AnimatorComponent::updateCB()
{
  auto& graphMan = g_graphicsMan();

  if(!m_boneTransformCB) {
    m_boneTransformCB = graphMan.createConstantBuffer(sizeof(BoneTransformCB));
  }

  for(uint32 i = 0; i < m_finalBoneTransforms.size(); ++i) {
    m_boneTransformData.finalTransforms[i] = m_finalBoneTransforms[i].getTransposed();
  }
  graphMan.updateConstantBuffer(m_boneTransformCB,
                                &m_boneTransformData,
                                sizeof(BoneTransformCB));
}
}
