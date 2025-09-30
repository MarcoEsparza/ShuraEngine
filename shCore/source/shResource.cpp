/*****************************************************************************/
/*
*  @file    shResource.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/09/29
*  @brief   Resource base class.
*
*  Resource base class.
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
#include "shResource.h"

namespace shEngineSDK {
void
Resource::setName(const String& name)
{
  m_name = name;
}
}
