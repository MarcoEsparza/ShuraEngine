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

#if SH_PLATFORM == SH_PLATFORM_WIN32
# if SH_COMPILER == SH_COMPILER_MSVC
#  if defined(SH_STATIC_LIB)
#   define SH_PHYSICS_EXPORT
#  else
#   if defined (SH_PHYSICS_EXPORTS)
#    define SH_PHYSICS_EXPORT __declspec(dllexport)
#   else
#    define SH_PHYSICS_EXPORT __declspec(dllimport)
#   endif
#  endif
# else // Any other compiler
#  if defined(SH_STATIC_LIB)
#   define SH_PHYSICS_EXPORT
#  else
#   if defined(SH_PHYSICS_EXPORTS)
#    define SH_PHYSICS_EXPORT __attribute__((dllexport))
#   else
#    define SH_PHYSICS_EXPORT __attribute__((dllimport))
#   endif
#  endif
# endif
# define SH_PHYSICS_HIDDEN
#else
# define SH_PHYSICS_EXPORT __attribute__((visibility("default")))
# define SH_PHYSICS_HIDDEN __attribute__((visibility("hidden")))
#endif
