/*****************************************************************************/
/*
*  @file    shGraphicsTypes.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/16
*  @brief   Structs and enums for graphics manager.
*
*  Structs and enums for graphics manager.
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
#include <shVector2.h>
#include <shVector3.h>
#include <shVector4.h>
#include <shMatrix4.h>

namespace shEngineSDK {
/*****************************************************************************/
/*
*  Forward declarations
*/
/*****************************************************************************/

class Texture2D;

/**
*  @brief Sample descriptor.
*/
struct SH_CORE_EXPORT SampleDesc
{
  /**
  *  @brief Default constructor for SampleDesc.
  */
  SampleDesc() = default;

  /**
  *  @brief Constructor for SampleDesc with parameters.
  */
  SampleDesc(uint32 _count, uint32 _quality)
  {
    count = _count;
    quality = _quality;
  }

  /**
  *  @brief Copy constructor for SampleDesc.
  */
  uint32 count = 0;

  /**
  *  @brief Copy constructor for SampleDesc.
  */
  uint32 quality = 0;
};

/**
*  @brief Enumerator for suppoerted graphic apis.
*/
namespace GRAPHIC_API {
enum E
{
  kDX11 = 0,                                // DirectX 11 API
  kOGL = 1,                                 // OpenGL API
};
}

/**
*  @brief Input Layout types enumerator namespace.
*/
namespace INPUT_LAYOUT_TYPES {
enum E
{
  kPosition = 0,                       // Position in 3D space
  kNormal,                             // Normal vector for lighting calculations
  kTexcoord,                           // Texture coordinates for mapping textures
  kTangents,                           // Tangent vector for normal mapping
  kBitangents,                         // Bitangent vector for normal mapping
  kBoneIndices,                        // Indices of bones affecting the vertex
  kBoneWieghts,                        // Weights of bones affecting the vertex
  kColor = 8,                          // Color information for the vertex
};
}

/**
*  @brief Vertex struct.
*/
struct SH_CORE_EXPORT VertexData
{
  /**
  *  @brief Position in 3D space.
  */
  Vector3 position = Vector3(0.0f, 0.0f, 0.0f);

  /**
  *  @brief Normal vector for lighting calculations.
  */
  Vector3 normal = Vector3(0.0f, 0.0f, 0.0f);

  /**
  *  @brief Texture coordinates for mapping textures.
  */
  Vector2 tex = Vector2(0.0f, 0.0f);

  /**
  *  @brief Tangent vector for normal mapping.
  */
  Vector3 tangents = Vector3(0.0f, 0.0f, 0.0f);

  /**
  *  @brief Bitangent vector for normal mapping.
  */
  Vector3 bitangents = Vector3(0.0f, 0.0f, 0.0f);

  /**
  *  @brief Indices of bones affecting the vertex.
  */
  Vector4 boneIds = Vector4(-1.0f, -1.0f, -1.0f, -1.0f);

  /**
  *  @brief Weights of bones affecting the vertex.
  */
  Vector4 boneWeights = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
};

/**
*  @brief Vertex struct for GUI
*/
struct SH_CORE_EXPORT GUIVertexData
{
  /**
  *  @brief Position in 2D space.
  */
  Vector2 position = Vector2(0.0f, 0.0f);

  /**
  *  @brief Texture coordinates for mapping textures.
  */
  Vector2 texcoord = Vector2(0.0f, 0.0f);

  /**
  *  @brief Color information for the vertex.
  */
  uint32 color = 0;
};

/**
*  @brief Viewport structure
*/
struct SH_CORE_EXPORT Viewport
{
  /**
  *  @brief Width of the viewport.
  */
  float width = 0.0f;

  /**
  *  @brief Height of the viewport.
  */
  float height = 0.0f;

  /**
  *  @brief Minimum depth of the viewport.
  */
  float minDepth = 0.0f;

  /**
  *  @brief Maximum depth of the viewport.
  */
  float maxDepth = 0.0f;

  /**
  *  @brief Top-left X coordinate of the viewport.
  */
  float topLeftX = 0.0f;

  /**
  *  @brief Top-left Y coordinate of the viewport.
  */
  float topLeftY = 0.0f;
};

/**
*  @brief Filters enumerator.
*/
namespace SAMPLER_FILTER {
enum E
{
  // Point sampling for minification, magnification, and mipmapping
  kFilterMinMagMipPoint = 0,
  // Linear sampling for minification, magnification, and mipmapping
  kFilterMinMagMipLinear = 21,
  // Anisotropic filtering for improved quality at oblique angles
  kFilterAnisotropic = 85
};
}

/**
*  @brief Address mode.
*/
namespace TEXTURE_ADDRESS_MODE {
enum E
{
  kWrap = 1,         // Wraps texture coordinates to repeat the texture
  kMirror,           // Mirrors texture coordinates to create a mirrored effect
  kClamp,            // Clamps texture coordinates to the edge of the texture
  kBorder,           // Uses a border color for texture coordinates outside the range [0, 1]
  kMirrorOnce        // Mirrors texture coordinates once, then clamps them
};
}

/**
*  @brief Current supported formats.
*/
namespace TEXTURE_FORMAT {
enum E
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
enum E
{
  kVertexBuffer            = 0x1L,         // Vertex buffer for input assembly stage
  kIndexBuffer             = 0x2L,         // Index buffer for input assembly stage
  kConstantBuffer          = 0x4L,         // Constant buffer for shader stages
  kShaderResource          = 0x8L,         // Shader resource for read-only access in shaders
  kStreamOutput            = 0x10L,        // Stream output for geometry shader stage
  kRenderTarget            = 0x20L,        // Render target for output merger stage
  kDepthStencil            = 0x40L,        // Depth-stencil buffer for output merger stage
  kUnorderedAccess         = 0x80L,        // Unordered access view for compute shader stage
  kDecoder                 = 0x200L,       // Decoder for video processing
  kVideoEncoder            = 0x400L        // Video encoder for video processing
};
}

/**
*  @brief Usage enumerator.
*/
namespace USAGE {
enum E
{
  kDefault = 0,  // Default usage for resources that are frequently accessed by the GPU
  kImmutable,    // Immutable usage for resources that are created once and not modified
  kDynamic,      // Dynamic usage for resources that are frequently updated by the CPU
  kStaging // Staging usage for resources that are used for data transfer between CPU and GPU
};
}

/**
*  @brief Supported topology.
*/
namespace PRIMITIVE_TOPOLOGY {
enum E
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
enum E
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
  /**
  *  @brief Type of the input layout.
  */
  INPUT_LAYOUT_TYPES::E type;

  /**
  *  @brief Format of the input layout.
  */
  uint32 format;

  /**
  *  @brief Size of the input layout.
  */
  uint32 size;
};

/**
*  @brief Texture type for resource.
*/
namespace TEXTURE_TYPE {
enum E
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
enum E
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
enum E
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
