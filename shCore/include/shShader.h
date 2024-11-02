/*************************************************************/
/*
*  @file    shShader.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/26
*  @brief   Engine Shader class.
*
*  Engine Shader class.
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

namespace shEngineSDK {
/**
*  @brief Engine Shader class.
*/
class ProgramShader
{
 public:
  /**
  *  @brief Default constructor.
  */
   ProgramShader() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~ProgramShader() = default;
};
}
