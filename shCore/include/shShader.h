/*****************************************************************************/
/*
*  @file    shShader.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/14
*  @brief   Engine Shader class.
*
*  Engine Shader class.
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

/**
*  @brief Engine Geometry Shader class.
*/
class GeometryShader
{
 public:
  /**
  *  @brief Default constructor.
  */
  GeometryShader() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~GeometryShader() = default;
};

/**
*  @brief Engine Compute Shader class.
*/
class ComputeShader
{
 public:
  /**
  *  @brief Default constructor.
  */
  ComputeShader() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~ComputeShader() = default;
};
}
