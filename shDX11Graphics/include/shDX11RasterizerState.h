/*************************************************************/
/*
*  @file    shDX11RasterizerState.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/14
*  @brief   DirectX11 Rasterizer State.
*
*  DirectX11 Rasterizer State.
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
#include "shRasterizerState.h"

namespace shEngineSDK {
/**
*  @brief DirectX11 Rasterizer State.
*/
class DX11RasterizerState : public RasterizerState
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11RasterizerState() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11RasterizerState();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief DirectX11 Rasterizer State pointer.
  */
  ID3D11RasterizerState* m_pRasterS = nullptr;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE DX11RasterizerState::~DX11RasterizerState()
{
  SafeRelease(m_pRasterS);
}
}
