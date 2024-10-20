/*************************************************************/
/*
*  @file    shTexture.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief   Texture clss wrappers.
*
*  Texture clss wrappers.
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
class DX11Texture;

/**
*  @brief Texture wrapper.
*/
using Texture = DX11Texture;

class DX11Texture2D;

/**
*  @brief Texture2D wrapper.
*/
using Texture2D = DX11Texture2D;
#endif

}
