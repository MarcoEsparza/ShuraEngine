/*************************************************************/
/*
*  @file    shGraphicsTypes.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/02
*  @brief   Structs and enums for graphics manager.
*
*  Structs and enums for graphics manager.
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
#include "shVector2.h"
#include "shVector3.h"
#include "shVector4.h"
#include "shMatrix4.h"

namespace shEngineSDK {
/**
*  @brief Sample descriptor.
*/
struct SampleDesc
{
  uint32 count = 0;
  uint32 quality = 0;
};

/**
*  @brief Input Layout types enumerator namespace.
*/
namespace INPUT_LAYOUT_TYPES {
enum E
{
  kPosition = 0,
  kNormal,
  kTexcoord,
  kBoneIndices,
  kBoneWieghts
};
}

/**
*  @brief Vertex struct.
*/
struct VertexData
{
  Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 normal = Vector3(0.0f, 0.0f, 0.0f);
  Vector2 tex = Vector2(0.0f, 0.0f);
  Vector4 boneIds = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
  Vector4 boneWeights = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
};

/**
*  @brief View struct.
*/
struct WorldViewProjection
{
  Matrix4 world;
  ViewMatrix view;
  ProjectionMatrix proj;
};
}
