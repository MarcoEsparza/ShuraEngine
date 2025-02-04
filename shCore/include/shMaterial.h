/*************************************************************/
/*
*  @file    shMaterial.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/04
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
  *  @brief Material cast shadows.
  */
  bool m_bCastShadows = true;

  /**
  *  @brief Material receive shadows.
  */
  bool m_bReceiveShadows = true;

  /**
  *  @brief Material type.
  */
  MATERIAL_TYPE::E m_type;
};

/**
*  @brief PBR Material class.
*/
class SH_CORE_EXPORT PBRMaterial : public Material
{
 public:
  /**
  *  @brief Default constructor.
  */
  PBRMaterial() : Material(MATERIAL_TYPE::kPBR) {}

  /**
  *  @brief Default destructor.
  */
  ~PBRMaterial() = default;

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
};
}
