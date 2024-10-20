/*************************************************************/
/*
*  @file    shDevice.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief   Device and Device Context class wrappers.
*
*  Device and Device Context class wrappers.
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
#include "shPrerequisitesCore.h"

namespace shEngineSDK {

#if SH_PLATFORM == SH_PLATFORM_WIN32
class DX11Device;

/**
*  @brief Device wrapper.
*/
using Device = DX11Device;

class DX11DeviceContext;

/**
*  @brief DeviceContext wrapper.
*/
using DeviceContext = DX11DeviceContext;

#endif

}
