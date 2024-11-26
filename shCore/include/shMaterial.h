/*************************************************************/
/*
*  @file    sh.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/
*  @brief   
*
*  
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
class SH_CORE_EXPORT Material
{
 public:
  Material() = default;
  virtual ~Material() = default;

  String name;
  SPtr<ProgramShader> shader;
};

class SH_CORE_EXPORT PBRMaterial : public Material
{
  PBRMaterial() = default;
  ~PBRMaterial() = default;

  SPtr<Texture2D> baseColor;
  SPtr<Texture2D> metallic;
  SPtr<Texture2D> roughness;
  SPtr<Texture2D> normal;
};
}
