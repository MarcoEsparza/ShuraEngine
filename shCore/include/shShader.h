/*************************************************************/
/*
*  @file    shShader.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/21
*  @brief   Engine Shader classes.
*
*  Engine Shader classes.
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
*  @brief Engine Vertex Shader class.
*/
class VertexShader
{
 public:
  /**
  *  @brief Default constructor.
  */
   VertexShader() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~VertexShader() = default;
};

/**
*  @brief Engine Pixel Shader class.
*/
class PixelShader
{
public:
  /**
  *  @brief Default constructor.
  */
  PixelShader() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~PixelShader() = default;
};
}
