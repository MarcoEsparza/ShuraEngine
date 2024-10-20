/*************************************************************/
/*
*  @file    shSwapChain.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief   SwapChain class wrapper.
*
*  SwapChain class wrapper.
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
class DX11SwapChain;

/**
*  @brief SwapChain wrapper.
*/
using SwapChain = DX11SwapChain;
#endif

}
