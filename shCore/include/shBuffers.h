/*****************************************************************************/
/*
*  @file    shBuffers.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/11/05
*  @brief   Engine Buffer classes.
*
*  Engine Buffer classes.
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
*  @brief Engine Vertex Buffer class.
*/
class VertexBuffer
{
 public:
  /**
  *  @brief Default constructor.
  */
  VertexBuffer() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~VertexBuffer() = default;
};

/**
*  @brief Engine Index Buffer class.
*/
class IndexBuffer
{
 public:
  /**
  *  @brief Default constructor.
  */
  IndexBuffer() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~IndexBuffer() = default;
};

/**
*  @brief Engine Constant Buffer class.
*/
class ConstantBuffer
{
 public:
  /**
  *  @brief Default constructor.
  */
  ConstantBuffer() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~ConstantBuffer() = default;
};
}
