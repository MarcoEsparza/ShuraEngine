/*************************************************************/
/*
*  @file    shPrerequisitesCore.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/06
*  @brief   Includes the utilities basic prerequisites and the core's.
*
*  Includes the utilities basic prerequisites and the core's.
* 
*  @bug     No bug known.
*/
/*************************************************************/
#pragma once

#include "shPrerequisitesUtilities.h"

#if defined (SH_CORE_EXPORTS)
# define SH_CORE_EXPORT __declspec(dllexport)
#else
# define SH_CORE_EXPORT __declspec(dllimport)
#endif
