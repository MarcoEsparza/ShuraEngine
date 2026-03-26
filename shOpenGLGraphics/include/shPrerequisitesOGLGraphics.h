/*************************************************************/
/*
*  @file    shPrerequisitesOGLGraphics.h
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
#include "GL/glew.h"
//#include "GL/GLU.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32
#include "GL/wglew.h"
#elif SH_PLATFORM == SH_PLATFORM_LINUX
#include "GL/glxew.h"
#endif
