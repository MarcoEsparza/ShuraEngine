/*************************************************************/
/*
*  @file    shDX11Shader.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/18
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

namespace shEngineSDK {
/**
*  @brief DirectX11 Shader base.
*/
class DX11Shader
{
 public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE DX11Shader() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11Shader();

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
};

/**
*  @brief DirectX11 Vertex Shader class wrapper.
*/
class DX11VertexShader : public DX11Shader
{
 public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE DX11VertexShader() = default;

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
  *  @brief DirectX11 Vertex Shader pointer.
  */
  ID3D11VertexShader* m_pShader = nullptr;
};

/**
*  @brief DirectX11 Pixel Shader class wrapper.
*/
class DX11PixelShader : public DX11Shader
{
 public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE DX11PixelShader() = default;

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
  *  @brief DirectX11 Pixel Shader pointer.
  */
  ID3D11PixelShader* m_pShader = nullptr;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE DX11Shader::~DX11Shader()
{
  SafeRelease(m_pBlob);
}

FORCEINLINE DX11VertexShader::~DX11VertexShader()
{
  SafeRelease(m_pShader);
}

FORCEINLINE DX11PixelShader::~DX11PixelShader()
{
  SafeRelease(m_pShader);
}
}
