/*************************************************************/
/*
*  @file    shDX11SwapChain.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/18
*  @brief   DirectX11 Swap Chain class wrapper.
*
*  DirectX11 Swap Chain class wrapper.
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
*  @brief DirectX11 Swap Chain class wrapper.
*/
class DX11SwapChain
{
 public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE DX11SwapChain() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11SwapChain();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief DirectX11 Swap Chain pointer.
  */
  IDXGISwapChain* m_pSwapChain = nullptr;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE DX11SwapChain::~DX11SwapChain()
{
  SafeRelease(m_pSwapChain);
}
}
