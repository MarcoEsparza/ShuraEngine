/*************************************************************/
/*
*  @file    shDX11Device.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/18
*  @brief   DirectX11 Device and DeviceContext class wrappers.
*
*  DirectX11 Device and DeviceContext class wrappers.
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
*  @brief DirectX11 Device wrapper.
*/
class DX11Device
{
 public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE DX11Device() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11Device();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief DirectX11 Device pointer.
  */
  ID3D11Device* m_pDevice = nullptr;
};

/**
*  @brief DirectX11 Device Context wrapper.
*/
class DX11DeviceContext
{
 public:
  /**
  *  @brief Default constructor.
  */
  FORCEINLINE DX11DeviceContext() = default;

  /**
  *  @brief Destructor. Release gpu memory.
  */
  FORCEINLINE virtual ~DX11DeviceContext();

  friend class DX11GraphicsManager;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 protected:
  /**
  *  @brief DirectX11 Device Context pointer.
  */
  ID3D11DeviceContext* m_pDeviceContext = nullptr;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE DX11Device::~DX11Device()
{
  SafeRelease(m_pDevice);
}

FORCEINLINE DX11DeviceContext::~DX11DeviceContext()
{
  SafeRelease(m_pDeviceContext);
}
}
