/*****************************************************************************/
/*
*  @file    shPlatformTypes.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/12
*  @brief   Defines basic platform types
*
*  Define the basic platform type variable. For porting, this might
*  be the place to start.
* 
* @bug      No bug known
*/
/*****************************************************************************/

#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/

#include <cstdint>
#include <cstdlib>
#include "shPlatformDefines.h"

/*
* @brief Here we define a "intermediate" language. If anything changes it
*        changes here.
*/
namespace shEngineSDK {
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::size_t;

/*****************************************************************************/
/*
*  Basic unsigned types
*/
/*****************************************************************************/

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

/*****************************************************************************/
/*
*  Basic signed types
*/
/*****************************************************************************/

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

/*****************************************************************************/
/*
*  Character types
*/
/*****************************************************************************/
#if SH_COMPILER == SH_COMPILER_MSVC
 using WCHAR = wchar_t;
#else
 using WCHAR = unsigned short;
#endif
using ANSICHAR = char;  // ANSI character type
using UNICHAR = WCHAR;  // UNICODE character type
using  unchar = unsigned char;

/*****************************************************************************/
/*
*  NULL data type
*/
/*****************************************************************************/
using TYPE_OF_NULL = int32;

/*****************************************************************************/
/*
*  SIZE_T is an arquitecture dependent data type
*/
/*****************************************************************************/
using SIZE_T = std::size_t;

/*****************************************************************************/
/*
*  Unique identifier type. This can be used for any kind of ID system, such as
*  object IDs, resource IDs, etc. The actual size of the UID can be adjusted
*  based on the needs of the engine. For example, if we want to support a large
*  number of objects, we might want to use a 64-bit integer for the UID. If we
*  want to save memory and we know that we won't have more than 4 billion objects, we
*  can use a 32-bit integer for the UID.
*/
/*****************************************************************************/

/**
*  @brief Small unique identifier type. 32 bits id.
*/
using UID = uint32;

/**
*  @brief Large unique identifier type. 64 bits id.
*/
using UUID = uint64;
}
