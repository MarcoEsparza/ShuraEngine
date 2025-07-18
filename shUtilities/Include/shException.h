/*****************************************************************************/
/*
*  @file    shException.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/16
*  @brief   Base exception class
*
*  Base exception class
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
*  @brief Base exception class
*/
class Exception : public std::exception
{
  /**
  *  @brief Default constructor
  */
  Exception() = default;

  /**
  *  @brief Constructor to fill member info
  * 
  *  @param const char* _type
  *  @param String _desc
  * 
  *  @param String _src
  */
  Exception(const char* _type, const String& _desc, const String& _src)
    : m_type(_type),
      m_desc(_desc),
      m_source(_src)
  {}

 protected:
  String m_type;
  String m_desc;
  String m_source;
};

#define SH_EXCEPT(type, desc)                                             \
{                                                                         \
  SH_ASSERT((std::is_base_of<shEngineSDK::Exception, type>::value),       \
    "Invalid exception type (" #type ") for SH_EXCEPT macro."             \
    "It needs to derive from shEngineSDK::Exception.");                   \
    throw type(desc, __PRETTY_FUNCTION__, __FILE__, __LINE__);            \
}

/**
*  @brief Used to avoid warnings and errors for unreferenced parameters.
*/
#define SH_UNREFERENCED_PARAMETER(x) (void)(x)
}
