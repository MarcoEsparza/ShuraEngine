/*****************************************************************************/
/*
*  @file    shStringID.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/05/13
*  @brief   Class for ID generetion from Strings.
*
*  Class for ID generetion from Strings.
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
*  @brief Class for ID generetion from Strings.
*/
class StringID
{
 public:
  /**
  *  @brief Default constructor.
  */
  StringID() = default;

  /**
  *  @brief Constructor from a String.
  * 
  *  @param String& str
  */
  StringID(const String& str) : m_string(str) {}

  /**
  *  @brief Default destructor.
  */
  ~StringID() = default;

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief ID generation.
  * 
  *  @return uint32
  */
  FORCEINLINE uint32
  getID() const;

  FORCEINLINE UID
  getUID() const;

  FORCEINLINE UUID
  getUUID() const;

  /**
  *  @brief Sets the string.
  * 
  *  @param String& str
  */
  FORCEINLINE void
  setString(const String& str);

  /**
  *  @brief Gets the storaged string.
  * 
  *  @return String
  */
  FORCEINLINE String
  getString() const;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 private:
  /**
  *  @brief String for id generation.
  */
  String m_string;
};

/*****************************************************************************/
/*
*  Implementations
*/
/*****************************************************************************/

FORCEINLINE uint32
StringID::getID() const
{
  return static_cast<uint32>(sh_hash<String>{}(m_string));
}

FORCEINLINE UID
StringID::getUID() const
{
  return static_cast<UID>(sh_hash<String>{}(m_string));
}

FORCEINLINE UUID
StringID::getUUID() const
{
  return static_cast<UUID>(sh_hash<String>{}(m_string));
}

FORCEINLINE void
StringID::setString(const String& str)
{
  m_string = str;
}

FORCEINLINE String
StringID::getString() const
{
  return m_string;
}
}
