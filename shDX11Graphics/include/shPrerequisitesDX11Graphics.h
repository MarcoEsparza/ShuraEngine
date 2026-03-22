/*************************************************************/
/*
*  @file    shPrerequisitesDX11Graphics.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/18
*  @brief   Includes the utilities, core and graphics basic prerequisites.
*
*  Includes the utilities, core and graphics basic prerequisites.
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

#if SH_PLATFORM == SH_PLATFORM_WIN32

#include "d3d11.h"

namespace shEngineSDK {
template <class T>
void SafeRelease(T * &p){
  if (p != nullptr) {
    p->Release();
    p = nullptr;
  }
}
}

#endif
