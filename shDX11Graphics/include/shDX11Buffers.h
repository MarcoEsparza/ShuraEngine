/*************************************************************/
/*
*  @file    shDX11Buffers.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/18
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

namespace shEngineSDK {
/**
*  @brief DirectX11 base buffer.
*/
class DX11Buffer
{
 public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE DX11Buffer() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11Buffer();

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

/**
*  @brief DirectX11 Vertex Buffer class wrapper.
*/
class DX11VertexBuffer : public DX11Buffer
{
 public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE DX11VertexBuffer() = default;

  /**
  *  @brief Default destructor.
  */
  FORCEINLINE ~DX11VertexBuffer() = default;

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief Buffer stride.
  */
  uint32 m_stride = 0;
};

/**
*  @brief DirectX11 Index Buffer class wrapper.
*/
class DX11IndexBuffer : public DX11Buffer
{
 public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE DX11IndexBuffer() = default;

  /**
  *  @brief Default destructor.
  */
  FORCEINLINE ~DX11IndexBuffer() = default;

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief Buffer format.
  */
  uint32 m_dataFormat = DXGI_FORMAT_R32_UINT;
};

/**
*  @brief DirectX11 Constant Buffer class wrapper.
*/
class DX11ConstantBuffer : public DX11Buffer
{
public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE DX11ConstantBuffer() = default;

  /**
  *  @brief Default destructor.
  */
  FORCEINLINE ~DX11ConstantBuffer() = default;

  friend class DX11GraphicsManager;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE DX11Buffer::~DX11Buffer()
{
  SafeRelease(m_pBuffer);
}
}
