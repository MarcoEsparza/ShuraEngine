/*************************************************************/
/*
*  @file    shPrerequisitesPhysics.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/11
*  @brief   Includes the utilities basic prerequisites and define physics
*           export.
*
*  Includes the utilities basic prerequisites and define physics export.
*
*  @bug     No bug known.
*/
/*************************************************************/
#pragma once

#include "shPrerequisitesUtilities.h"

#if defined (SH_PHYSICS_EXPORTS)
# define SH_PHYSICS_EXPORT __declspec(dllexport)
#else
# define SH_PHYSICS_EXPORT __declspec(dllimport)
#endif
