/*****************************************************************************/
/*
*  @file    shFileExplorer.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/08/06
*  @brief   File Explorer class platform independent.
*
*  File Explorer class platform independent.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shFileExplorer.h"
#include <shException.h>

#if SH_PLATFORM == SH_PLATFORM_LINUX

namespace shEngineSDK{
bool
FileExplorer::openFile(String& outPath,
                       const String& filter,
                       const String& initialDir)
{
  SH_UNREFERENCED_PARAMETER(outPath);
  SH_UNREFERENCED_PARAMETER(filter);
  SH_UNREFERENCED_PARAMETER(initialDir);
  return false;
}

bool
FileExplorer::openFolder(String&, const String&)
{
  return false;
}

FileExplorer&
g_fileExplorer()
{
  return FileExplorer::instance();
}
}

#endif
