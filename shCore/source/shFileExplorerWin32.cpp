/*****************************************************************************/
/*
*  @file    shFileExplorer.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/13
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

#define MAX_PATH_LENGTH 260

#if SH_PLATFORM == SH_PLATFORM_WIN32

#include "Windows.h"

namespace shEngineSDK {
const String FileExplorer::DEFAULT_ASSETS_FOLDER = "resources/EngineData/DefaultAssets/";
const String FileExplorer::ASSETS_FOLDER = "resources/Assets/";
const String FileExplorer::SHADERS_FOLDER = "resources/EngineData/Shaders/";
const String FileExplorer::DEFAULT_FONTS_FOLDER = "resources/EngineData/Fonts/";

bool
FileExplorer::openFile(String& outPath,
                       const String& filter,
                       const String& initialDir)
{
  OPENFILENAMEA ofn;       // common dialog box structure
  CHAR szFile[MAX_PATH_LENGTH] = { 0 }; // buffer for file name
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
FileExplorer::saveFile(String& outPath,
                       const String& filter,
                       const String& initialDir)
{
  OPENFILENAMEA ofn;       // common dialog box structure
  CHAR szFile[MAX_PATH_LENGTH] = { 0 }; // buffer for file name
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
  ofn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
  // Display the Save dialog box.
  if (GetSaveFileNameA(&ofn) == TRUE) {
    outPath = ofn.lpstrFile;
    return true;
  }
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