/*************************************************************/
/*
*  @file    shOGLBuffers.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/27
*  @brief   OpenGL buffer class wrappers.
*
*  OpenGL buffer class wrappers.
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
  FORCEINLINE ~OGLVertexBuffer();

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
  uint32 m_vbo = 0;
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
  FORCEINLINE ~OGLIndexBuffer();

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
  uint32 m_ibo = 0;
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
  FORCEINLINE ~OGLConstantBuffer();

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
  uint32 m_ubo = 0;

  /**
  *  @brief Buffer size.
  */
  uint32 m_binding = 0;
};

FORCEINLINE OGLVertexBuffer::~OGLVertexBuffer()
{
  glDeleteBuffers(1, &m_vbo);
}

FORCEINLINE OGLIndexBuffer::~OGLIndexBuffer()
{
  glDeleteBuffers(1, &m_ibo);
}

FORCEINLINE OGLConstantBuffer::~OGLConstantBuffer()
{
  glDeleteBuffers(1, &m_ubo);
}
}
