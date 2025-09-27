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
class Texture2D;

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
      uint32 bInvertRoughness : 1;
      uint32 Unused : 17;
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
  *  @brief Constructor for material type.
  */
  //Material(MATERIAL_TYPE::E type) : m_type(type) {}

  /**
  *  @brief Default destructor.
  */
  virtual ~Material() = default;

 public:
  /**
  *  @brief Material name.
  */
  //String name;

  /**
  *  @brief Material type.
  */
  //MATERIAL_TYPE::E m_type = MATERIAL_TYPE::kPBR;

  /**
  *  @brief Material properties.
  */
  MaterialProperties m_properties = {};

  /**
  *  @brief Base color texture.
  */
  SPtr<Texture2D> baseColor;

  /**
  *  @brief Metallic texture.
  */
  SPtr<Texture2D> metallic;

  /**
  *  @brief Roughness texture.
  */
  SPtr<Texture2D> roughness;

  /**
  *  @brief Normal texture.
  */
  SPtr<Texture2D> normal;

  /**
  *  @brief Ambient occlusion texture.
  */
  SPtr<Texture2D> ao;

  /**
  *  @brief Path of the base color texture.
  */
  String baseColorPath;

  /**
  *  @brief Path of the normal texture.
  */
  String normalPath;

  /**
  *  @brief Path of the metallic texture.
  */
  String metallicPath;

  /**
  *  @brief Path of the roughness texture.
  */
  String roughnessPath;

  /**
  *  @brief Path of the ao texture.
  */
  String aoPath;

  Vector3 baseColorFactor = Vector3(1.0f, 1.0f, 1.0f);
  Vector2 metallicRoughnessFactor = Vector2(1.0f, 1.0f);
};
}
