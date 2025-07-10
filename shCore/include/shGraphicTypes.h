/*************************************************************/
/*
*  @file    shGraphicsTypes.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/06/04
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
class Texture2D;

/**
*  @brief Sample descriptor.
*/
struct SH_CORE_EXPORT SampleDesc
{
  SampleDesc() = default;

  SampleDesc(uint32 _count, uint32 _quality)
  {
    count = _count;
    quality = _quality;
  }

  uint32 count = 0;
  uint32 quality = 0;
};

/**
*  @brief Enumerator for suppoerted graphic apis.
*/
namespace GRAPHIC_API {
enum SH_CORE_EXPORT E
{
  kDX11 = 0,
  kOGL
};
}

/**
*  @brief Input Layout types enumerator namespace.
*/
namespace INPUT_LAYOUT_TYPES {
enum SH_CORE_EXPORT E
{
  kPosition = 0,
  kNormal,
  kTexcoord,
  kTangents,
  kBitangents,
  kBoneIndices,
  kBoneWieghts,
  kColor
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
  Vector3 tangents = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 bitangents = Vector3(0.0f, 0.0f, 0.0f);
  Vector4 boneIds = Vector4(-1.0f, -1.0f, -1.0f, -1.0f);
  Vector4 boneWeights = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
};

/**
*  @brief Vertex struct for GUI
*/
struct SH_CORE_EXPORT GUIVertexData
{
  Vector2 position = Vector2(0.0f, 0.0f);
  Vector2 texcoord = Vector2(0.0f, 0.0f);
  uint32 color = 0;
};

/**
*  @brief Viewport structure
*/
struct SH_CORE_EXPORT Viewport
{
  float width = 0.0f;
  float height = 0.0f;
  float minDepth = 0.0f;
  float maxDepth = 0.0f;
  float topLeftX = 0.0f;
  float topLeftY = 0.0f;
};

/**
*  @brief Filters enumerator.
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
*  @brief Address mode.
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
*  @brief Current supported formats.
*/
namespace TEXTURE_FORMAT {
enum SH_CORE_EXPORT E
{
  kUnknown                                                = 0,
  kR32G32B32A32_TYPELESS                                  = 1,
  kR32G32B32A32_FLOAT                                     = 2,
  kR32G32B32A32_UINT                                      = 3,
  kR32G32B32A32_SINT                                      = 4,
  kR32G32B32_TYPELESS                                     = 5,
  kR32G32B32_FLOAT                                        = 6,
  kR32G32B32_UINT                                         = 7,
  kR32G32B32_SINT                                         = 8,
  kR16G16B16A16_TYPELESS                                  = 9,
  kR16G16B16A16_FLOAT                                     = 10,
  kR16G16B16A16_UNORM                                     = 11,
  kR16G16B16A16_UINT                                      = 12,
  kR16G16B16A16_SNORM                                     = 13,
  kR16G16B16A16_SINT                                      = 14,
  kR32G32_TYPELESS                                        = 15,
  kR32G32_FLOAT                                           = 16,
  kR32G32_UINT                                            = 17,
  kR32G32_SINT                                            = 18,
  kR32G8X24_TYPELESS                                      = 19,
  kD32_FLOAT_S8X24_UINT                                   = 20,
  kR32_FLOAT_X8X24_TYPELESS                               = 21,
  kX32_TYPELESS_G8X24_UINT                                = 22,
  kR10G10B10A2_TYPELESS                                   = 23,
  kR10G10B10A2_UNORM                                      = 24,
  kR10G10B10A2_UINT                                       = 25,
  kR11G11B10_FLOAT                                        = 26,
  kR8G8B8A8_TYPELESS                                      = 27,
  kR8G8B8A8_UNORM                                         = 28,
  kR8G8B8A8_UNORM_SRGB                                    = 29,
  kR8G8B8A8_UINT                                          = 30,
  kR8G8B8A8_SNORM                                         = 31,
  kR8G8B8A8_SINT                                          = 32,
  kR16G16_TYPELESS                                        = 33,
  kR16G16_FLOAT                                           = 34,
  kR16G16_UNORM                                           = 35,
  kR16G16_UINT                                            = 36,
  kR16G16_SNORM                                           = 37,
  kR16G16_SINT                                            = 38,
  kR32_TYPELESS                                           = 39,
  kD32_FLOAT                                              = 40,
  kR32_FLOAT                                              = 41,
  kR32_UINT                                               = 42,
  kR32_SINT                                               = 43,
  kR24G8_TYPELESS                                         = 44,
  kD24_UNORM_S8_UINT                                      = 45,
  kR24_UNORM_X8_TYPELESS                                  = 46,
  kX24_TYPELESS_G8_UINT                                   = 47,
  kR8G8_TYPELESS                                          = 48,
  kR8G8_UNORM                                             = 49,
  kR8G8_UINT                                              = 50,
  kR8G8_SNORM                                             = 51,
  kR8G8_SINT                                              = 52,
  kR16_TYPELESS                                           = 53,
  kR16_FLOAT                                              = 54,
  kD16_UNORM                                              = 55,
  kR16_UNORM                                              = 56,
  kR16_UINT                                               = 57,
  kR16_SNORM                                              = 58,
  kR16_SINT                                               = 59
};
}

/**
*  @brief Flags.
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
*  @brief Usage enumerator.
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
*  @brief Supported topology.
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

/**
*  @brief Clear flags.
*/
namespace CLEAR_FLAGS {
enum SH_CORE_EXPORT E
{
  kDepth = 1,
  kStencil
};
}

/**
*  @brief InputLayout descriptor struct.
*/
struct SH_CORE_EXPORT InputDesc
{
  INPUT_LAYOUT_TYPES::E type;
  uint32 format;
  uint32 size;
};

/**
*  @brief Texture type for resource.
*/
namespace TEXTURE_TYPE {
enum SH_CORE_EXPORT E
{
  kNone = 0,
  kDiffuse,
  kSpecular,
  kAmbient,
  kEmissive,
  kHeight,
  kNormals,
  kShininess,
  kOpacity,
  kDisplacement,
  kLightMap,
  kReflection,
  kBaseColor,
  kNormalCamera,
  kEmissionColor,
  kMetalness,
  kDiffuseRoughness,
  kAmbientOcclusion,
  kUnknown,
  kSheen,
  kClearCoat,
  kTransmission,

  kCount
};
}

/**
*  @brief Blend options.
*/
namespace BLEND {
enum SH_CORE_EXPORT E
{
  kZero = 1,
  kOne = 2,
  kSrcColor = 3,
  kInvSrcColor = 4,
  kSrcAlpha = 5,
  kInvSrcAlpha = 6,
  kDestAlpha = 7,
  kInvDestAlpha = 8,
  kDestColor = 9,
  kInvDestColor = 10,
  kSrcAlphaSat = 11,
  kBlendFactor = 14,
  kInvBlendFactor = 15,
  kSrc1Color = 16,
  kInvSrc1Color = 17,
  kSrc1Alpha = 18,
  kInvSrc1Alpha = 19
};
}

/**
*  @brief Blend operations.
*/
namespace BLEND_OP {
enum SH_CORE_EXPORT E
{
  kAdd = 1,
  kSubtract = 2,
  kRevSubtract = 3,
  kMin = 4,
  kMax = 5
};
}

/**
*  @brief Blend descriptions for rendertargets.
*/
struct SH_CORE_EXPORT RenderTarget_BlendDesc
{
  bool blendEnable;
  BLEND::E srcBlend;
  BLEND::E destBlend;
  BLEND_OP::E blendOp;
  BLEND::E srcBlendAlpha;
  BLEND::E destBlendAlpha;
  BLEND_OP::E blendOpAlpha;
  uint8 renderTargetWriteMask;
};

/**
*  @brief Blend descriptor.
*/
struct SH_CORE_EXPORT BlendDesc
{
  bool alphaToCoverageEnable;
  bool independentBlendEnable;
  RenderTarget_BlendDesc renderTarget[8];
};

/**
*  @brief Enables for color white.
*/
namespace COLOR_WHITE_ENABLE {
enum E
{
  kEnableRed = 1,
  kEnableGreen = 2,
  kEnableBlue = 4,
  kEnableAlpha = 8,
  kEnableAll = (((kEnableRed | kEnableGreen) | kEnableBlue) | kEnableAlpha)
};
}

/**
*  @brief Rasterizer fill mode.
*/
namespace FILL_MODE {
enum E
{
  kWireframe = 2,
  kSolid = 3
};
}

/**
*  @brief Rasterizer cull mode.
*/
namespace CULL_MODE {
enum E
{
  kNone = 1,
  kFront = 2,
  kBack = 3
};
}

/**
*  @brief Rasterizer descriptor.
*/
struct RasterizerDesc
{
  FILL_MODE::E fillMode;
  CULL_MODE::E cullMode;
  bool frontCounterClockwise;
  int32 depthBias;
  float depthBiasClamp;
  float slopeScaledDepthBias;
  bool depthClipEnable;
  bool scissorEnable;
  bool multisampleEnable;
  bool antialiasedLineEnable;
};

/**
*  @brief Comparison function for depth stencil state.
*/
namespace COMPARISON_FUNC {
enum E
{
  kNever = 1,
  kLess = 2,
  kEqual = 3,
  kLessEqual = 4,
  kGreater = 5,
  kNotEqual = 6,
  kGreaterEqual = 7,
  kAlways = 8
};
}

/**
*  @brief Depth write mask.
*/
namespace DEPTH_WRITE_MASK {
enum E
{
  kZero = 0,
  kAll = 1
};
}

/**
*  @brief Type of operation for Stencil.
*/
namespace STENCIL_OP {
enum E
{
  kKeep = 1,
  kZero = 2,
  kReplace = 3,
  kIncrSat = 4,
  kDecrSat = 5,
  kInvert = 6,
  kIncr = 7,
  kDecr = 8
};
}

/**
*  @brief Operation descriptor for depth stencil state.
*/
struct DepthStencilOpDesc
{
  STENCIL_OP::E stencilFailOp;
  STENCIL_OP::E stencilDepthFailOp;
  STENCIL_OP::E stencilPassOp;
  COMPARISON_FUNC::E stencilFunc;
};

/**
*  @brief Depth Stencil State decriptor.
*/
struct DepthStencilDesc
{
  bool depthEnable;
  DEPTH_WRITE_MASK::E depthWriteMask;
  COMPARISON_FUNC::E depthFunc;
  bool stencilEnable;
  uint8 stencilReadMask;
  uint8 stencilWriteMask;
  DepthStencilOpDesc frontFace;
  DepthStencilOpDesc backFace;
};

/**
*  @brief Shader macro struct.
*/
struct SH_CORE_EXPORT ShaderMacro
{
  String name;
  String definition;
};

struct SH_CORE_EXPORT RenderTarget
{
  RenderTarget() = default;
  RenderTarget(const WPtr<Texture2D>& pRT, uint32 mipLevel = 0)
    : pRenderTarget(pRT), mipLevel(mipLevel)
  {}

  WPtr<Texture2D> pRenderTarget;
  uint32 mipLevel = 0;
};

struct SH_CORE_EXPORT UnorderedAccess
{
  UnorderedAccess() = default;
  UnorderedAccess(const WPtr<Texture2D>& pUA, int32 mipLevel = 0)
    : pUAccess(pUA), mipLevel(mipLevel)
  {}

  WPtr<Texture2D> pUAccess;
  int32 mipLevel = 0;
};
}
