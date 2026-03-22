/*****************************************************************************/
/*
*  @file    shDX11BlendState.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/02/06
*  @brief   DirectX11 Blend State.
*
*  DirectX11 Blend State.
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
#include <shBlendState.h>
#include <shLinearColor.h>

#if SH_PLATFORM == SH_PLATFORM_WIN32

namespace shEngineSDK {
/**
*  @brief DirectX11 Blend State.
*/
class DX11BlendState : public BlendState
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11BlendState() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  virtual ~DX11BlendState();

  void
  setDebugName(const String& name) override;

  friend class DX11GraphicsManager;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 protected:
  /**
  *  @brief DirectX11 Blend State pointer.
  */
  ID3D11BlendState* m_pBlendS = nullptr;

  /**
  *  @brief Blend factor color.
  */
  LinearColor m_blendFactor;
};
}

#endif
