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

#if SH_PLATFORM == SH_PLATFORM_WIN32
# if SH_COMPILER == SH_COMPILER_MSVC
#  if defined(SH_STATIC_LIB)
#   define SH_CORE_EXPORT
#  else
#   if defined (SH_CORE_EXPORTS)
#    define SH_CORE_EXPORT __declspec(dllexport)
#   else
#    define SH_CORE_EXPORT __declspec(dllimport)
#   endif
#  endif
# else // Any other compiler
#  if defined(SH_STATIC_LIB)
#   define SH_CORE_EXPORT
#  else
#   if defined(SH_CORE_EXPORTS)
#    define SH_CORE_EXPORT __attribute__((dllexport))
#   else
#    define SH_CORE_EXPORT __attribute__((dllimport))
#   endif
#  endif
# endif
# define SH_CORE_HIDDEN
#else
# define SH_CORE_EXPORT __attribute__((visibility("default")))
# define SH_CORE_HIDDEN __attribute__((visibility("hidden")))
#endif
