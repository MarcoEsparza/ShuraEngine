/*****************************************************************************/
/*
*  @file    shPrerequisitesFMODAudio.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/16
*  @brief   Includes the utilities, core and fmod prerequisites.
*
*  Includes the utilities and fmod prerequisites.
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
#include <shPrerequisitesCore.h>

namespace shEngineSDK {
template <class T>
void
safeRelease(T*& p)
{
  if (p != nullptr) {
    p->release();
    p = nullptr;
  }
}
}

