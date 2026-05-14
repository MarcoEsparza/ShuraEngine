/*****************************************************************************/
/*
*  @file    shPath.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/14
*  @brief   Path class, to use for paths in the disk.
*
*  Path class, to use for paths in the disk.
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
#include "shPrerequisitesUtilities.h"

namespace shEngineSDK {
/**
*  @brief Path class, to use for paths in the disk.
*/
class SH_UTILITY_EXPORT Path
{
 public:
  /**
  *  @brief Default constructor.
  */
  Path() = default;

  /**
  *  @brief Constructor from string.
  * 
  *  @param String& filePath
  */
  explicit Path(const String& filePath);

  /**
  *  @brief Constructor from const char*.
  * 
  *  @param const char* filePath
  */
  explicit Path(const char* filePath);

  /**
  *  @brief Default destructor.
  */
  ~Path() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Gets the content of the full path as string.
  * 
  *  @return String: Full path as string.
  */
  String
  string() const;

  /**
  *  @brief Gets the content of the full path as wstring.
  * 
  *  @return WString: Full path as wstring.
  */
  WString
  wstring() const;

  /**
  *  @brief Gets the content of the full path as const char*.
  * 
  *  @return const char*: Full path as const char*.
  */
  const char*
  c_str() const;

  /**
  *  @brief Gets the content of the full path as const wchar_t*.
  * 
  *  @return const wchar_t*: Full path as const wchar_t*.
  */
  const WCHAR*
  w_str() const;

  /**
  *  @brief Replaces the extension for the extension given.
  * 
  *  @param const String& ext: New extension to replace, including the dot,
  *                            for example: ".png".
  */
  void
  replaceExtension(const String& ext);

  /**
  *  @brief Replaces the filename for the filename given.
  * 
  *  @param const String& filename: New filename to replace, excluding the directory,
  *                                 for example: "file.png".
  */
  void
  replaceFilename(const String& filename);

  /**
  *  @brief Gets the extension of the Path.
  * 
  *  @return String: Extension of the Path, including the dot, for example: ".png".
  */
  String
  extension() const;

  /**
  *  @brief Checks if the Path has extension.
  * 
  *  @return bool: True if the Path has extension, false if it doesn't.
  */
  bool
  hasExtension() const;

  /**
  *  @brief Checks if the Path has a directory.
  * 
  *  @return bool: True if the Path has a directory, false if it doesn't.
  */
  bool
  hasDirectory() const;

  /**
  *  @brief Checks if the Path has a root directory.
  * 
  *  @return bool: True if the Path has a root directory, false if it doesn't.
  */
  bool
  hasRoot() const;

  /**
  *  @brief Gets only the filename, excluding the directory.
  * 
  *  @return String: Filename of the Path, excluding the directory, for example: "file.png".
  */
  String
  filename() const;

  /**
  *  @brief Gets only the directory, excluding the filename.
  * 
  *  @return String: Directory of the Path, excluding the filename,
  *                  for example: "resources/textures".
  */
  String
  directory() const;

  /**
  *  @brief Gets the parent directory of the Path, excluding the filename and the
  *         last directory.
  * 
  *  @return String: Parent directory of the Path.
  */
  String
  parentPath() const;

  /**
  *  @brief Compare a Vector of extensions to checks if theres at least one equal
  *         to the path extension.
  * 
  *  @param Vector<String>& extensions: Vector of extensions to compare, including the dot,
  *                                     for example: {".png", ".bmp"}.
  *
  *  @return bool: True if a extension is equal, false if none was equal.
  */
  bool
  compareExtensions(const Vector<String>& extensions) const;

  /**
  *  @brief Checks if the Path is empty.
  * 
  *  @return bool: True if the Path is empty, false if it isn't.
  */
  bool
  empty() const;

  /**
  *  @brief Clears the content of the Path, making it empty.
  */
  void
  clear();

  /**
  *  @brief Swaps the content of this Path with the content of other Path.
  * 
  *  @param Path& path: Path to swap with.
  */
  void
  swap(Path& path);

  /***************************************************************************/
  /*
  *  Operator overload
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Operator to assign the content of other Path to this.
  * 
  *  @param Path& other: Path to assign to this.
  * 
  *  @return Path&: This Path with the content of other Path assigned.
  */
  FORCEINLINE Path&
  operator=(const Path& other);

  /**
  *  @brief Operator to assign the content of a string to this Path,
  *         parsing the string as a path.
  * 
  *  @param String& other: String to assign to this Path, parsing it as a path.
  * 
  *  @return String: The full path as string, after parsing the string and
  *                  assigning it to this Path.
  */
  FORCEINLINE String
  operator=(const String& other);

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 private:
  /**
  *  @brief Full path, including directory and filename.
  */
  String m_fullPath;

  /**
  *  @brief Extension of the path, including the dot, for example: ".png".
  */
  String m_extension;

  /**
  *  @brief Filename of the path, excluding the directory, for example: "file.png".
  */
  String m_filename;

  /**
  *  @brief Directory of the path, excluding the filename, for example: "resources/textures".
  */
  String m_directory;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FORCEINLINE Path&
Path::operator=(const Path& other)
{
  m_fullPath = other.string();
  m_extension = other.extension();
  m_filename = other.filename();
  m_directory = other.directory();
  return *this;
}

FORCEINLINE String
Path::operator=(const String& other)
{
  Path tempPath(other);
  *this = tempPath;
  return m_fullPath;
}
}
