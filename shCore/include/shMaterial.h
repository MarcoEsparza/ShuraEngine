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
  *  @brief Default destructor.
  */
  virtual ~Material() = default;

 public:
  /**
  *  @brief Material name.
  */
  String name;

  /**
  *  @brief Material shader.
  */
  SPtr<ProgramShader> shader;
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
  PBRMaterial() = default;

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
};
}
