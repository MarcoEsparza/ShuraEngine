/*****************************************************************************/
/*
*  @file    shDX11InputLayout.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/02/07
*  @brief   DirectX11 Input Layout class wrapper.
*
*  DirectX11 Input Layout class wrapper.
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
#include <shInputLayout.h>

namespace shEngineSDK {
/**
*  @brief DirectX11 Input Layout class wrapper.
*/
class DX11InputLayout : public InputLayout
{
 public:
  /**
  *  @brief Default constructor.
  */
  DX11InputLayout() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  virtual ~DX11InputLayout();

  friend class DX11GraphicsManager;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 protected:
  /**
  *  @brief DirectX11 Input Layout pointer.
  */
  ID3D11InputLayout* m_pLayout = nullptr;
};
}
