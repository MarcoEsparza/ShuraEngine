/*****************************************************************************/
/*
*  @file    shFileExplorer.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/15
*  @brief   File Explorer class platform independent.
*
*  File Explorer class platform independent.
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
#include "shPrerequisitesCore.h"
#include <shModule.h>

namespace shEngineSDK {
class SH_CORE_EXPORT FileExplorer : public Module<FileExplorer>
{
 public:
  /**
  *  @brief Default constructor.
  */
  FileExplorer() = default;

  /**
  *  @brief Default destructor.
  */
  virtual ~FileExplorer() = default;

 public:
  /**
  *  @brief Opens a file explorer window to select a file.
  *
  *  @param outPath The path of the selected file will be stored here.
  *  @param filter The filter for the file types (e.g., "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0").
  *  @param initialDir The initial directory to open in the file explorer.
  *
  *  @return true if a file was selected, false otherwise.
  */
  bool
  openFile(String& outPath,
           const String& filter = "All Files (*.*)\0*.*\0",
           const String& initialDir = "");

  /**
  *  @brief Opens a file explorer window to select a file for saving.
  * 
  *  @param outPath The path of the selected file will be stored here.
  *  @param filter The filter for the file types
  *         (e.g., "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0").
  *  @param initialDir The initial directory to open in the file explorer.
  * 
  *  @return true if a file was selected, false otherwise.
  */ 
  bool
  saveFile(String& outPath,
           const String& filter = "All Files (*.*)\0*.*\0",
           const String& initialDir = "");

  /**
  *  @brief Opens a file explorer window to select a folder.
  *
  *  @param outPath The path of the selected folder will be stored here.
  *  @param initialDir The initial directory to open in the file explorer.
  *
  *  @return true if a folder was selected, false otherwise.
  */
  bool
  openFolder(String& outPath,
  const String& initialDir = "");

 public:
  static const String DEFAULT_ASSETS_FOLDER;
  static const String ASSETS_FOLDER;
  static const String SHADERS_FOLDER;
  static const String DEFAULT_FONTS_FOLDER;
  static const String CODECS_FOLDER;
};

/**
*  @brief Global access function for the FileExplorer module.
*/
SH_CORE_EXPORT FileExplorer&
g_fileExplorer();
}
