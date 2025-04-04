/*************************************************************/
/*
*  @file    shMaterial.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/11
*  @brief   Material classes.
*
*  Material classes.
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
#include "shTexture.h"
#include "shShader.h"

namespace shEngineSDK {
namespace MATERIAL_TYPE {
enum E
{
  kPBR = 0,
  kTranslucent
};
}

//MS_ALIGN(16)
struct MaterialProperties
{
  uint32 bHasDiffuseMap : 1;
  uint32 bHasSpecularMap : 1;
  uint32 bHasNormalMap : 1;
  uint32 bHasMetalnessMap : 1;
  uint32 bHasRoughnessMap : 1;
  uint32 bHasAmbientOcclusionMap : 1;
  uint32 bIsOpaque : 1;
  uint32 bHasAlphaTest : 1;
  uint32 bHasAlphaBlend : 1;
  uint32 bIsDoubleSided : 1;
  uint32 bWireframeEnabled : 1;
  uint32 bCanCastShadows : 1;
  uint32 bCanReceiveShadows : 1;
  uint32 Unused : 19;
}; /*GCC_ALIGN(16)*/

/**
*  @brief Matrial Base.
*/
class SH_CORE_EXPORT Material
{
 public:
  /**
  *  @brief Default constructor.
  */
  Material() = default;

  /**
  *  @brief Constructor for material type.
  */
  Material(MATERIAL_TYPE::E type) : m_type(type) {}

  /**
  *  @brief Default destructor.
  */
  virtual ~Material() = default;

 public:
  /**
  *  @brief Material name.
  */
  String name;

  /**
  *  @brief Material type.
  */
  MATERIAL_TYPE::E m_type = MATERIAL_TYPE::kPBR;

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

  String baseColorPath;
  String normalPath;
  String metallicPath;
  String roughnessPath;
  String aoPath;
};

/**
*  @brief PBR Material class.
*/
//class SH_CORE_EXPORT PBRMaterial : public Material
//{
// public:
//  /**
//  *  @brief Default constructor.
//  */
//  PBRMaterial() : Material(MATERIAL_TYPE::kPBR) {}
//
//  /**
//  *  @brief Default destructor.
//  */
//  virtual ~PBRMaterial()
//  {
//    baseColor.reset();
//    metallic.reset();
//    roughness.reset();
//    normal.reset();
//    ao.reset();
//  }
//
//  /**
//  *  @brief Base color texture.
//  */
//  SPtr<Texture2D> baseColor;
//
//  /**
//  *  @brief Metallic texture.
//  */
//  SPtr<Texture2D> metallic;
//
//  /**
//  *  @brief Roughness texture.
//  */
//  SPtr<Texture2D> roughness;
//
//  /**
//  *  @brief Normal texture.
//  */
//  SPtr<Texture2D> normal;
//
//  /**
//  *  @brief Ambient occlusion texture.
//  */
//  SPtr<Texture2D> ao;
//};
}
