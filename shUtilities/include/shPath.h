/*************************************************************/
/*
*  @file    shPath.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/15
*  @brief   Path class, to use for paths in the disk.
*
*  Path class, to use for paths in the disk.
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

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
 public:
  /**
  *  @brief Gets the content of the Path in const char*.
  * 
  *  @return const char*
  */
  const char*
  c_str() const;

  const wchar_t*
  wc_str() const;

  /**
  *  @brief Gets the content of the Path in string.
  * 
  *  @return String
  */
  String
  toString() const;

  /**
  *  @brief Replaces the extension for the extension given.
  * 
  *  @param String& ext
  */
  void
  replaceExtension(const String& ext);

  /**
  *  @brief Gets the extension of the Path.
  * 
  *  @return String
  */
  String
  extension() const;

  /**
  *  @brief Checks if the Path has extension.
  * 
  *  @return bool
  */
  bool
  hasExtension() const;

  /**
  *  @brief Gets only the filename, excluding the directory.
  * 
  *  @return String
  */
  String
  filename() const;

  /**
  *  @brief Compare a Vector of extensions to checks if theres at least one equal
  *         to the path extension.
  * 
  *  @param Vector<String>& extensions
  *
  *  @return bool, true if a extension is equal, false if none was equal.
  */
  bool
  compareExtensions(const Vector<String>& extensions) const;

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
 public:
  /**
  *  @brief Operator to add the content from other Path to this.
  *
  *  @param lValue-Path.
  *  @param rValue-Path.
  *
  *  @return Path
  */
  FORCEINLINE Path
  operator+(const Path& other) const;

  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 private:
  /**
  *  @brief The path.
  */
  SystemPath m_path;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE Path
Path::operator+(const Path& other) const
{
  String ret = m_path.string() + other.toString();
  return Path(ret);
}
}
