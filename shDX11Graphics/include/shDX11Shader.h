/*************************************************************/
/*
*  @file    shDX11Shader.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/26
*  @brief   DirectX11 Program Shader class wrapper.
*
*  DirectX11 Program Shader class wrapper.
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
*  @brief DirectX11 Program Shader class wrapper.
*/
class DX11ProgramShader : public ProgramShader
{
public:
  /**
  *  @brief Default constructor.
  */
  DX11ProgramShader() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11ProgramShader();

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
  ID3DBlob* m_pVertexBlob = nullptr;

  /**
  *  @brief DirectX11 Blob pointer.
  */
  ID3DBlob* m_pPixelBlob = nullptr;

  /**
  *  @brief DirectX11 Vertex Shader pointer.
  */
  ID3D11VertexShader* m_pVertexShader = nullptr;

  /**
  *  @brief DirectX11 Pixel Shader pointer.
  */
  ID3D11PixelShader* m_pPixelShader = nullptr;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE DX11ProgramShader::~DX11ProgramShader()
{
  SafeRelease(m_pVertexBlob);
  SafeRelease(m_pPixelBlob);
  SafeRelease(m_pVertexShader);
  SafeRelease(m_pPixelShader);
}
}
