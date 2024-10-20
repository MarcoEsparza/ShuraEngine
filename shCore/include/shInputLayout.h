/*************************************************************/
/*
*  @file    shInputLayout.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief   InputLayout class wrapper.
*
*  InputLayout class wrapper.
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
class DX11InputLayout;

/**
*  @brief InputLayout wrapper.
*/
using InputLayout = DX11InputLayout;
#endif

}
