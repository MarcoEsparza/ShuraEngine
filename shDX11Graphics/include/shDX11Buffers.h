/*************************************************************/
/*
*  @file    shDX11Buffers.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/21
*  @brief   DirectX11 Vertex Buffer, Index Buffer and Constant Buffer class wrappers.
*
*  DirectX11 Vertex Buffer, Index Buffer and Constant Buffer class wrappers.
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
#include "shPrerequisitesDX11Graphics.h"
#include "shBuffers.h"

namespace shEngineSDK {
/**
*  @brief DirectX11 Vertex Buffer class wrapper.
*/
class DX11VertexBuffer : public VertexBuffer
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11VertexBuffer() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11VertexBuffer();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief DirectX11 Buffer pointer.
  */
  ID3D11Buffer* m_pBuffer = nullptr;

  /**
  *  @brief Buffer stride.
  */
  uint32 m_stride = 0;
};

/**
*  @brief DirectX11 Index Buffer class wrapper.
*/
class DX11IndexBuffer : public IndexBuffer
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11IndexBuffer() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11IndexBuffer();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief DirectX11 Buffer pointer.
  */
  ID3D11Buffer* m_pBuffer = nullptr;

  /**
  *  @brief Buffer format.
  */
  uint32 m_dataFormat = DXGI_FORMAT_R32_UINT;
};

/**
*  @brief DirectX11 Constant Buffer class wrapper.
*/
class DX11ConstantBuffer : public ConstantBuffer
{
public:
  /**
  *  @brief Default constructor.
  */
  DX11ConstantBuffer() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11ConstantBuffer();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
protected:
  /**
  *  @brief DirectX11 Buffer pointer.
  */
  ID3D11Buffer* m_pBuffer = nullptr;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE DX11VertexBuffer::~DX11VertexBuffer()
{
  SafeRelease(m_pBuffer);
}

FORCEINLINE DX11IndexBuffer::~DX11IndexBuffer()
{
  SafeRelease(m_pBuffer);
}

FORCEINLINE DX11ConstantBuffer::~DX11ConstantBuffer()
{
  SafeRelease(m_pBuffer);
}
}
