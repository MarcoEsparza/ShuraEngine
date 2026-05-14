/*****************************************************************************/
/*
*  @file    shPath.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/14
*  @brief   Path class, to use for every path in the disk.
*
*  Path class, to use for every path in the disk.
*
*  @bug     No bug known.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPath.h"

namespace shEngineSDK {
static String
getFileExtension(const String& filePath)
{
  SIZE_T lastDotPos = filePath.find_last_of('.');
  if (lastDotPos == String::npos) {
    return ""; // No extension found
  }
  return filePath.substr(lastDotPos, filePath.length() - lastDotPos);
}

static String
getFilename(const String& filePath)
{
  SIZE_T lastSlashPos = filePath.find_last_of("/\\");
  if (lastSlashPos == String::npos) {
    return filePath; // No directory found, return the whole path
  }
  return filePath.substr(lastSlashPos + 1, filePath.length() - lastSlashPos - 1);
}

static String
getDirectory(const String& filePath)
{
  SIZE_T lastSlashPos = filePath.find_last_of("/\\");
  if (lastSlashPos == String::npos) {
    return ""; // No directory found
  }
  return filePath.substr(0, lastSlashPos);
}

static WString
stringToWString(const String& str)
{
  return WString(str.begin(), str.end());
}

static void
toLower(String& str)
{
  std::transform(str.begin(), str.end(), str.begin(),
    [](unsigned char c)
    {
      return cast::st<char>(::tolower(c));
    });
}

Path::Path(const String& filePath)
{
  m_fullPath = filePath;
  m_extension = getFileExtension(filePath);
  m_filename = getFilename(filePath);
  m_directory = getDirectory(filePath);
}

Path::Path(const char* filePath)
{
  m_fullPath = filePath;
  m_extension = getFileExtension(m_fullPath);
  m_filename = getFilename(m_fullPath);
  m_directory = getDirectory(m_fullPath);
}

String
Path::string() const
{
  return m_fullPath;
}

WString
Path::wstring() const
{
  return stringToWString(m_fullPath);
}

const char*
Path::c_str() const
{
  return m_fullPath.c_str();
}

const WCHAR*
Path::w_str() const
{
  const WCHAR* wstr = stringToWString(m_fullPath).c_str();
  return wstr;
}

void
Path::replaceExtension(const String& ext)
{
  if (!hasExtension()) {
    m_fullPath += ext;
  }
  else {
    SIZE_T lastDotPos = m_fullPath.find_last_of('.');
    m_fullPath = m_fullPath.substr(0, lastDotPos) + ext;
  }

  m_extension = ext;
}

void
Path::replaceFilename(const String& filename)
{
  if (!hasDirectory()) {
    m_fullPath = filename;
  }
  else {
    SIZE_T lastSlashPos = m_fullPath.find_last_of("/\\");
    m_fullPath = m_fullPath.substr(0, lastSlashPos + 1) + filename;
  }
  m_filename = filename;
}

String
Path::extension() const
{
  return m_extension;
}

bool
Path::hasExtension() const
{
  return !m_extension.empty();
}

bool
Path::hasDirectory() const
{
  return !m_directory.empty();
}

bool
Path::hasRoot() const
{
  return !m_fullPath.empty() && (m_fullPath[0] == '/' || m_fullPath[0] == '\\');
}

String
Path::filename() const
{
  return m_filename;
}

String
Path::directory() const
{
  return m_directory;
}

String
Path::parentPath() const
{
  if (!hasDirectory()) {
    return "";
  }
  SIZE_T lastSlashPos = m_directory.find_last_of("/\\");
  if (lastSlashPos == String::npos) {
    return ""; // No parent directory found
  }
  return m_directory.substr(0, lastSlashPos);
}

bool
Path::compareExtensions(const Vector<String>& extensions) const
{
  String lowerExt = m_extension;
  toLower(lowerExt);

  for (const String& ext : extensions) {
    String lowerExtItem = ext;
    toLower(lowerExtItem);
    if (lowerExt == lowerExtItem) {
      return true;
    }
  }

  return false;
}

bool
Path::empty() const
{
  return m_fullPath.empty();
}

void
Path::clear()
{
  m_fullPath.clear();
  m_extension.clear();
  m_filename.clear();
  m_directory.clear();
}

void
Path::swap(Path& path)
{
  String newPath = path.string();
  m_fullPath.swap(newPath);
  m_extension = getFileExtension(m_fullPath);
  m_filename = getFilename(m_fullPath);
  m_directory = getDirectory(m_fullPath);
}
}
