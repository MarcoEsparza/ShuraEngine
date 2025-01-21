/*************************************************************/
/*
*  @file    shDX11DepthStencilState.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/15
*  @brief   DirectX11 Depth Stencil State.
*
*  DirectX11 Depth Stencil State.
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
#include "shDepthStencilState.h"

namespace shEngineSDK {
/**
*  @brief DirectX11 Depth Stencil State.
*/
class DX11DepthStencilState : public DepthStencilState
{
 public:
  /**
  *  @brief Default constructor.
  */
   DX11DepthStencilState() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11DepthStencilState();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief DirectX11 Depth Stencil State pointer.
  */
  ID3D11DepthStencilState* m_pDepthSS = nullptr;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE DX11DepthStencilState::~DX11DepthStencilState()
{
  SafeRelease(m_pDepthSS);
}
}
