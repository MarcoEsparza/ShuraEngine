/*************************************************************/
/*
*  @file    shPath.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/12/16
*  @brief   Path class, to use for every path in the disk.
*
*  Path class, to use for every path in the disk.
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
#include "shPath.h"

namespace shEngineSDK {
Path::Path(const String& filePath)
{
  m_path = filePath;
}

Path::Path(const char* filePath)
{
  m_path = filePath;
}

const char*
Path::c_str() const
{
  String ret = m_path.string();
  return ret.c_str();
}

String
Path::toString() const
{
  return m_path.string();
}

void
Path::replaceExtension(const String& ext)
{
  m_path.replace_extension(ext);
}

String
Path::extension() const
{
  return m_path.extension().string();
}

bool
Path::hasExtension() const
{
  return m_path.has_extension();
}

String
Path::filename() const
{
  return m_path.filename().string();
}

bool
Path::compareExtensions(const Vector<String>& extensions) const
{
  for (auto& ext : extensions) {
    if (ext == m_path.extension()) {
      return true;
    }
  }

  return false;
}
}
