/*************************************************************/
/*
*  @file    shPlatformDefines.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/09/04
*  @brief   Include the most basic defines for platform
*
*  Include the most basic defines for platform
*/
/*************************************************************/

#pragma once
/*************************************************************/
/*
*  Initial platform/compiler-related stuf to set
*/
/*************************************************************/

#define SH_PLATFORM_WIN32       1                 // Windows Platform
#define SH_PLATFORM_LINUX       2                 // Linux Platform
#define SH_PLATFORM_OSX         3

#define SH_COMPILER_MSVC        1                 // Visual Studio Compiler
#define SH_COMPILER_GNUC        2                 // GCC Compiler
#define SH_COMPILER_INTEL       3                 // Intel Compiler
#define SH_COMPILER_CLANG       4                 // Clang Compiler

#define SH_ARQUITECTURE_X86_32  1                 // Intel x86 32 bits
#define SH_ARQUITECTURE_X86_64  2                 // Intel x86 64 bits

#define SH_ENDIAN_LITTLE        1
#define SH_ENDIAN_BIG           2

// Define the actual endian type (little endian for Windows, Linux, Apple and PS4)

#define SH_ENDIAN SH_ENDIAN_LITTLE

#define SH_VERSION_MAJOR            0
#define SH_VERSION_MIMINOR          2
#define SH_VERSION_PATCH            0
#define SH_VERSION_BUILD            1

// Define if on create a we want to report warnings on unknown symbols

#define SH_DEBUG_DETAILED_SYMBOLS   1

/*************************************************************/
/*
*
*/
/*************************************************************/

#if defined(__clang__)
#   define SH_COMPILER SH_COMPILER_CLANG
#   define SH_COMP_VER __cland_version__
#   define SH_THREADLOCAL __thread
#   define SH_STDCALL __attribute__((stdcall))
#   define SH_CDECL __attribute__((cdecl))
#   define SH_FALLTHROUGH [[clang::fallthrough]]
#elif defined (__GNUC__)
#   define SH_COMPILER SH_COMPILER_GNUC
#   define SH_COMP_VER (((__GNUC__)*100)+(__GNUC_MINOR__*10)+__GNUC_PATCHLEVEL__)
#   define SH_THREADLOCAL __thread
#   define SH_STDCALL __attribute__((stdcall))
#   define SH_CDECL __attribute__((cdecl))
#   define SH_FALLTHROUGH __attribute__((fallthrough));
#elif defined (__INTEL_COMPILER)
#   define SH_COMPILER SH_COMPILER_INTEL
#   define SH_COMP_VER __INTEL_COMPILER
#   define SH_STDCALL __stdcall
#   define SH_CDECL __cdecl
#   define SH_FALLTHROUGH

/*
*  SH_THREADLOCAL define is down below because Intel compiler defines it
*  differently based on platform
*/

// Check after Clang end Intel, we could be building with either with VS
#elif defined(_MSC_VER)
#   define SH_COMPILER SH_COMPILER_MSVC
#   define SH_COMP_VER _MSC_VER
#   define SH_THREADLOCAL __declspec
#   define SH_STDCALL __stdcall
#   define SH_CDECL __cdecl
#   define SH_FALLTHROUGH
#   undef  __PRETTY_FUNCTION__
#   define __PRETTY_FUNCTION__ __FUNCSIG__
#else
// No know compiler found, send the error to the output (if any)
#   pragma error "No known compiler. "
#endif

/*************************************************************/
/*
*  See if we can use __forceinline or if we need to use __inline instead
*/
/*************************************************************/

#if SH_COMPILER == SH_COMPILER_MSVC
# if SH_COMP_VER >= 1200
#  define FORCEINLINE __forceinline
#  ifndef RESTRICT
#   define RESTRICT __restrict
#  endif
# endif
#elif defined (__MINGW32__)
# ifndef (FORCEINLINE)
#  define FORCEINLINE __inline
#  ifndef RESTRICT
#   define RESTRICT
#  endif
# endif
#else
# define FORCEINLINE __inline
# ifndef RESTRICT
#  define RESTRICT __restrict
# endif
#endif

/*************************************************************/
/*
* Finds the current platform
*/
/*************************************************************/

#if defined (__WIN32__) || defined (_WIN32)
# define SH_PLATFORM SH_PLATFORM_WIN32
#elif defined (__APPLE_CC__)
# define SH_PLATFORM SH_PLATFORM_OSX
#elif defined (__ORBIS__)
# define SH_PLATFORM SH_PLATFORM_LINUX
#else
// No know platform found, send the error to the output (if any)
#   pragma error "No known compiler. "
#endif

/*************************************************************/
/*
* Finds the arquitecture type
*/
/*************************************************************/

#if defined (__x86_x64__) || defined(_M_X64)
# define SH_ARCH_TYPE SH_ARQUITECTURE_X86_64
#else
# defined SH_ARCH_TYPE SH_ARQUITECTURE_X86_32
#endif

/*************************************************************/
/*
* Memory alignment macros
*/
/*************************************************************/

#if SH_COMPILER == SH_COMPILER_MSVC
# define MS_ALIGN(n) __declspec(align(n))
# ifndef GCC_PACK
#  define GCC_PACK(n)
# endif
#elif (SH_COMPILER ++ SH_COMPILER_GNUC)
# define MS_ALIGN(n)
# define GCC_PACK(n)
# define GCC_ALIGN(n) __attribute__((__aligned__(n)))
#else
# define MS_ALIGN
# define GCC_PACK(n) __attribute__((packed, aligned(n)))
# define GCC_ALIGN(n) __attribute__((__aligned__(n)))
#endif

/*************************************************************/
/*
* For throw override (deprecated on c++11 but VS does not have handle
*/
/*************************************************************/

#if SH_COMPILER == SH_COMPILER_MSVC
# define _NOEXCEPT noexcept
#elif SH_COMPILER == SH_COMPILER_INTEL
# define _NOEXCEPT noexcept
#elif SH_COMPILER == SH_COMPILER_GNUC
# define _NOEXCEPT noexcept
#else
# define _NOEXCEPT
#endif

/*************************************************************/
/*
* Library export specifics
*/
/*************************************************************/

#if SH_PLATFORM == SH_PLATFORMWIN32
# if SH_COMPILER == SH_COMPILER_MSVC
#  if defined(SH_STATIC_LIB)
#   define SH_UTILITY_EXPORT
#  else
#   if defined (SH_UTILITY_EXPORTS)
#    define SH_UTILITY_EXPORT __declspec(dllexport)
#   else
#    define SH_UTILITY_EXPORT __declspec(dllimport)
#   endif
#  endif
# else // Any other compiler
#  if defined(SH_STATIC_LIB)
#   define SH_UTILITY_EXPORT
#  else
#   if defined(SH_UTILITY_EXPORT)
#    define SH_UTILITY_EXPORT __attribute__(dllexport)
#   else
#    define SH_UTILITY_EXPORT __attribute__(dllimport)
#   endif
#  endif
# endif
# define SH_UTILITY_HIDDEN
#else
# define SH_UTILITY_EXPORT __attribute__((visibility("default")))
# define SH_UTILITY_HIDDEN __attribute__((visibility("hidden")))
#endif

// DLL export plug ins
#if SH_PLATFORM == SH_PLATFORM_WIN32
# if SH_COMPILER == SH_COMPILER_MSVC
#  define SH_PLUGIN_EXPORT __declspec(dllexport)
# else
#  define SH_PLUGIN_EXPORT __attribute__(dllexport)
# endif
#else
#  define SH_PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

/*************************************************************/
/*
* Windows specifics settings
*/
/*************************************************************/

// Win32
#if SH_PLATFORM == SH_PLATFORM_WIN32
# if defined(_DEBUG) || defined(DEBUG)
#  define SH_DEBUG_MODE 1
# else
#  define SH_DEBUG_MODE 0
# endif
# if SH_COMPILER == SH_COMPILER_INTEL
#  define SH_THREADLOCAL __declspec(thread)
# endif
#endif

/*************************************************************/
/*
* Linux-Apple specifics settings
*/
/*************************************************************/

#if SH_PLATFORM == SH_PLATFORM_LINUX || SH_PLATFORM_OSX

// If we're on debug mode
# if defined(_DEBUG) || defined(DEBUG)
#  define SH_DEBUG_MODE 1
# else
#  define SH_DEBUG_MODE 0
# endif
# if SH_COMPILER == SH_COMPILER_INTEL
#  define SH_TRHEADLOCAL thread
# endif
#endif

/*************************************************************/
/*
* Definition of debug macros
*/
/*************************************************************/

#if SH_DEBUG_MODE
# define SH_DEBUG_ONLY(x) x
# define SH_ASSERT(x) assert(x)
#else
# define CH_DEBUG_ONLY(x)
# define CH_ASSERT(x)
#endif

/*************************************************************/
/*
* Extern
*/
/*************************************************************/

#define SH_EXTERN extern "C"

/*************************************************************/
/*
* Disable some compiler warnings
*/
/*************************************************************/

// If we're compiling with Visual Studio
#if SH_COMPILER == SH_COMPILER_MSVC
 /*
 * TODO: This is not deactivated anywhere, therefore it applies to any file
 * that includes this header.
 */

 // Secure versions aren't multi-platform, so  we won't be using them
# define _CRT_SECURE_NO_WARNINGS

 /*
 * Disable: "<type> needs to have DLL interface to be used by clients"
 * Happens on STL member variables which are not public therefore is ok
 */

# pragma warning(disable: 4251)

 // Disable: 'X' Function call with parameters that may be unsafe
# pragma warning(disable: 4996)

/*
* Disable: decorated name length exceed, name was truncated. Happens with
* really long type names. Even fairly standard use of std: unordered_map
* with custom parameters, meaning I can't really do much to avoid it.
* It shouldn't effect execution but might cause problems if you compile
* library with one compiler and use it in another.
*/

# pragma warning(disable: 4503)

/*
* Disable: nonstandard extension used: override specifier 'keyword'.
* Happens when keyboard was used that is not the c++ standard, for
* example, one of the override specifiers that also works under /clr.
*/

//# pragma warning(disable: 4481)

#endif
