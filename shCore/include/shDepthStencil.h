/*************************************************************/
/*
*  @file    shDepthStencil.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief   DepthStencil class wrapper.
*
*  DepthStencil class wrapper.
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
class DX11DepthStencilView;

/**
*  @brief DepthStencil wrapper.
*/
using DepthStencil = DX11DepthStencilView;
#endif

}
