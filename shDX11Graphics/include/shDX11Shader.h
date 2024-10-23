/*************************************************************/
/*
*  @file    shDX11Shader.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/21
*  @brief   DirectX11 Vertex Shader and Pixel Shader class wrappers.
*
*  DirectX11 Vertex Shader and Pixel Shader class wrappers.
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
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11VertexShader();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief DirectX11 Blob pointer.
  */
  ID3DBlob* m_pBlob = nullptr;

  /**
  *  @brief DirectX11 Vertex Shader pointer.
  */
  ID3D11VertexShader* m_pShader = nullptr;
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
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11PixelShader();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief DirectX11 Blob pointer.
  */
  ID3DBlob* m_pBlob = nullptr;

  /**
  *  @brief DirectX11 Pixel Shader pointer.
  */
  ID3D11PixelShader* m_pShader = nullptr;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE DX11VertexShader::~DX11VertexShader()
{
  SafeRelease(m_pBlob);
  SafeRelease(m_pShader);
}

FORCEINLINE DX11PixelShader::~DX11PixelShader()
{
  SafeRelease(m_pBlob);
  SafeRelease(m_pShader);
}
}
