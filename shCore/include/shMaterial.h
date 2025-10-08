/*****************************************************************************/
/*
*  @file    shMaterial.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/09/24
*  @brief   Material classes.
*
*  Material classes.
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
#include <shVector3.h>
#include <shVector2.h>

namespace shEngineSDK {
//class Texture2D;
class ImageResource;

/**
*  @brief Matrial properties structure.
*/
struct SH_CORE_EXPORT MaterialProperties
{
  union
  {
    struct {
      uint32 bHasDiffuseMap : 1;
      uint32 bHasSpecularMap : 1;
      uint32 bHasNormalMap : 1;
      uint32 bHasMetalnessMap : 1;
      uint32 bHasRoughnessMap : 1;
      uint32 bHasAmbientOcclusionMap : 1;
      uint32 bHasEmissiveMap : 1;
      uint32 bIsOpaque : 1;
      uint32 bHasAlphaTest : 1;
      uint32 bHasAlphaBlend : 1;
      uint32 bIsDoubleSided : 1;
      uint32 bWireframeEnabled : 1;
      uint32 bCanCastShadows : 1;
      uint32 bCanReceiveShadows : 1;
      uint32 bInvertNormals : 1;
      uint32 bInvertRoughness : 1;
      uint32 bInvertMetalness : 1;
      uint32 bHasOpacityMask : 1;
      uint32 bUseEmission : 1;
      uint32 Unused : 13;
    } flags;
    uint32 value = 0;
  } properties;
};

/**
*  @brief Matrial Base.
*/
class SH_CORE_EXPORT Material : public Resource
{
 public:
  /**
  *  @brief Default constructor.
  */
  Material() : Resource(RESOURCE_TYPE::kMaterial) {}

  /**
  *  @brief Default destructor.
  */
  virtual ~Material() = default;

 public:

  /**
  *  @brief Material properties.
  */
  MaterialProperties m_properties = {};

  /**
  *  @brief Base color texture.
  */
  WPtr<ImageResource> m_baseColor;

  /**
  *  @brief Metallic texture.
  */
  WPtr<ImageResource> m_metalness;

  /**
  *  @brief Roughness texture.
  */
  WPtr<ImageResource> m_roughness;

  /**
  *  @brief Normal texture.
  */
  WPtr<ImageResource> m_normal;

  /**
  *  @brief Ambient occlusion texture.
  */
  WPtr<ImageResource> m_ao;

  WPtr<ImageResource> m_emissive;

  WPtr<ImageResource> m_specular;

  WPtr<ImageResource> m_opacityMask;

  Vector3 baseColorFactor = Vector3::ONE;
  Vector2 metallicRoughnessFactor = Vector2::ONE;
  Vector3 emissiveFactor = Vector3::ZERO;
  float emmisiveIntensity = 1.0f;
  float alphaCutoff = 0.5f;
};
}
