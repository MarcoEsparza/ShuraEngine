/*************************************************************/
/*
*  @file    shGraphicsTypes.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/05
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
struct SH_CORE_EXPORT SampleDesc
{
  uint32 count = 0;
  uint32 quality = 0;
};

/**
*  @brief Input Layout types enumerator namespace.
*/
namespace INPUT_LAYOUT_TYPES {
enum SH_CORE_EXPORT E
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
struct SH_CORE_EXPORT VertexData
{
  Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 normal = Vector3(0.0f, 0.0f, 0.0f);
  Vector2 tex = Vector2(0.0f, 0.0f);
  Vector4 boneIds = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
  Vector4 boneWeights = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
};

/**
*  @brief 
*/
namespace SAMPLER_FILTER {
enum SH_CORE_EXPORT E
{
  kFilterMinMagMipPoint = 0,
  kFilterMinMagMipLinear = 21,
  kFilterAnisotropic = 85
};
}

/**
*  @brief
*/
namespace TEXTURE_ADDRESS_MODE {
enum SH_CORE_EXPORT E
{
  kWrap = 1,
  kMirror,
  kClamp,
  kBorder,
  kMirrorOnce
};
}

/**
*  @brief
*/
namespace TEXTURE_FORMAT {
enum SH_CORE_EXPORT E
{
  kUnknown = 0,
  kR32G32B32A32_Typeless = 1,
  kR32G32B32A32_float = 2,
  kR32G32B32A32_uint = 3,
  kR32G32B32A32_sint = 4,
  kR32G32B32_Typeless = 5,
  kR32G32B32_float = 6,
  kR32G32B32_uint = 7,
  kR32G32B32_sint = 8,
  kR32G32_Typeless = 15,
  kR32G32_float = 16,
  kR32G32_uint = 17,
  kR32G32_sint = 18,
  kR8G8B8A8_Typeless = 27,
  kR8G8B8A8_unorm = 28,
  kR8G8B8A8_snorm = 29,
  kR8G8B8A8_uint = 30,
  kR8G8B8A8_sint = 32,
  kD24_UNORM_S8_uint = 45
};
}

/**
*  @brief
*/
namespace BIND_FLAGS {
enum SH_CORE_EXPORT E
{
  kVertexBuffer = 0x1L,
  kIndexBuffer = 0x2L,
  kConstantBuffer = 0x4L,
  kShaderResource = 0x8L,
  kStreamOutput = 0x10L,
  kRenderTarget = 0x20L,
  kDepthStencil = 0x40L,
  kUnorderedAccess = 0x80L,
  kDecoder = 0x200L,
  kVideoEncoder = 0x400L
};
}

/**
*  @brief
*/
namespace USAGE {
enum SH_CORE_EXPORT E
{
  kDefault = 0,
  kImmutable,
  kDynamic,
  kStaging
};
}

/**
*  @brief
*/
namespace PRIMITIVE_TOPOLOGY {
enum SH_CORE_EXPORT E
{
  kUndefined = 0,
  kPointList,
  kLineList,
  kLineStrip,
  kTrianglelist,
};
}

namespace CLEAR_FLAGS {
enum E
{
  kDepth = 1,
  kStencil
};
}
}
