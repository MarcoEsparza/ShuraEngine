/*****************************************************************************/
/*
*  @file    shOGLBuffers.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/17
*  @brief   OpenGL buffer class wrappers.
*
*  OpenGL buffer class wrappers.
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
#include "shPrerequisitesOGLGraphics.h"
#include "shBuffers.h"

namespace shEngineSDK {
/**
*  @brief OpenGL VertexBuffer class wrapper.
*/
class OGLVertexBuffer : public VertexBuffer
{
 public:
  /**
  *  @brief Default constructor.
  */
  OGLVertexBuffer() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~OGLVertexBuffer();

  friend class OGLGraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief OpenGL buffer.
  */
  uint32 m_bufferID = 0;
  uint32 m_stride = 0;
  uint32 m_size = 0;
};

/**
*  @brief OpenGL IndexBuffer class wrapper.
*/
class OGLIndexBuffer : public IndexBuffer
{
 public:
  /**
  *  @brief Default constructor.
  */
  OGLIndexBuffer() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~OGLIndexBuffer();

  friend class OGLGraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief OpenGL buffer.
  */
  uint32 m_bufferID = 0;
  uint32 m_count = 0;
};

/**
*  @brief OpenGL ConstantBuffer class wrapper.
*/
class OGLConstantBuffer : public ConstantBuffer
{
 public:
  /**
  *  @brief Default constructor.
  */
  OGLConstantBuffer() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~OGLConstantBuffer();

  friend class OGLGraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief OpenGL buffer.
  */
  uint32 m_bufferID = 0;

  /**
  *  @brief Buffer size.
  */
  uint32 m_size = 0;
};
}
