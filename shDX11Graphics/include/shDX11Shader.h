/*****************************************************************************/
/*
*  @file    shDX11Shader.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/04/14
*  @brief   DirectX11 Program Shader class wrapper.
*
*  DirectX11 Program Shader class wrapper.
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
#include "shPrerequisitesDX11Graphics.h"
#include "shShader.h"

namespace shEngineSDK {
/**
*  @brief DirectX11 Vertex Shader class wrapper.
*/
class DX11VertexShader : public VertexShader
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11VertexShader() = default;

  /**
  *  @brief Default destructor.
  */
  FORCEINLINE virtual ~DX11VertexShader();

  friend class DX11GraphicsManager;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 protected:
  /**
  *  @brief DirectX11 Blob pointer.
  */
  ID3DBlob* m_pBlob = nullptr;

  /**
  *  @brief DirectX11 Vertex Shader pointer.
  */
  ID3D11VertexShader* m_pVertexShader = nullptr;
};

/**
*  @brief DirectX11 Pixel Shader class wrapper.
*/
class DX11PixelShader : public PixelShader
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11PixelShader() = default;

  /**
  *  @brief Default destructor.
  */
  FORCEINLINE virtual ~DX11PixelShader();

  friend class DX11GraphicsManager;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 protected:
  /**
  *  @brief DirectX11 Blob pointer.
  */
  ID3DBlob* m_pBlob = nullptr;

  /**
  *  @brief DirectX11 Pixel Shader pointer.
  */
  ID3D11PixelShader* m_pPixelShader = nullptr;
};

/**
*  @brief DirectX11 Geometry Shader class wrapper.
*/
class DX11GeometryShader : public GeometryShader
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11GeometryShader() = default;

  /**
  *  @brief Default destructor.
  */
  FORCEINLINE virtual ~DX11GeometryShader();

  friend class DX11GraphicsManager;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 protected:
  /**
  *  @brief DirectX11 Blob pointer.
  */
  ID3DBlob* m_pBlob = nullptr;

  /**
  *  @brief DirectX11 Geometry Shader pointer.
  */
  ID3D11GeometryShader* m_pGeometryShader = nullptr;
};

/**
*  @brief DirectX11 Compute Shader class wrapper.
*/
class DX11ComputeShader : public ComputeShader
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11ComputeShader() = default;

  /**
  *  @brief Default destructor.
  */
  FORCEINLINE virtual ~DX11ComputeShader();

  friend class DX11GraphicsManager;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 protected:
  /**
  *  @brief DirectX11 Blob pointer.
  */
  ID3DBlob* m_pBlob = nullptr;

  /**
  *  @brief DirectX11 Compute Shader pointer.
  */
  ID3D11ComputeShader* m_pComputeShader = nullptr;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FORCEINLINE DX11VertexShader::~DX11VertexShader()
{
  SafeRelease(m_pBlob);
  SafeRelease(m_pVertexShader);
}

FORCEINLINE DX11PixelShader::~DX11PixelShader()
{
  SafeRelease(m_pBlob);
  SafeRelease(m_pPixelShader);
}

FORCEINLINE DX11GeometryShader::~DX11GeometryShader()
{
  SafeRelease(m_pBlob);
  SafeRelease(m_pGeometryShader);
}

FORCEINLINE DX11ComputeShader::~DX11ComputeShader()
{
  SafeRelease(m_pBlob);
  SafeRelease(m_pComputeShader);
}
}
