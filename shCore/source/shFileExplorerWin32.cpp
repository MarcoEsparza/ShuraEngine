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

#if SH_PLATFORM == SH_PLATFORM_WIN32

#include "Windows.h"

namespace shEngineSDK {
bool
FileExplorer::openFile(String& outPath,
                       const String& filter,
                       const String& initialDir)
{
  OPENFILENAMEA ofn;       // common dialog box structure
  CHAR szFile[260] = { 0 }; // buffer for file name
  // Initialize OPENFILENAME
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = nullptr;
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = filter.c_str();
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = nullptr;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = initialDir.empty() ? nullptr : initialDir.c_str();
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
  // Display the Open dialog box.
  if (GetOpenFileNameA(&ofn) == TRUE) {
    outPath = ofn.lpstrFile;
    return true;
  }
  return false;
}

bool
FileExplorer::openFolder(String& outPath, const String& initialDir)
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