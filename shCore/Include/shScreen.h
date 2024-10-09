/*************************************************************/
/*
*  @file    shScreen.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/08
*  @brief   Base screen
*
*  Base screen
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shScreenEventHandle.h"

namespace shEngineSDK {

#if SH_PLATFORM == SH_PLATFORM_WIN32
using PlatformScreen = void*;
#elif SH_PLATFORM == SH_PLATFORM_LINUX
using PlatformScreen = void*;
#endif

/**
*  @brief Structure for screen description.
*/
struct ScreenDesc
{
  /*************************************************************/
  /*
  *  App data
  */
  /*************************************************************/

  /**
  *  @brief Screen name.
  */
  String name = "";

  /**
  *  @brief Screen title.
  */
  String title = "";

  /**
  *  @brief The icon path to load it on init.
  */
  String iconPath = "";

  /*************************************************************/
  /*
  *  Transform
  */
  /*************************************************************/

  /**
  *  @brief Screen position in the X axis.
  */
  uint32 positionX = 0;

  /**
  *  @brief Screen position in the Y axis.
  */
  uint32 positionY = 0;

  /**
  *  @brief Screen widht.
  */
  uint32 width = 0;

  /**
  *  @brief Screen height.
  */
  uint32 height = 0;

  /*************************************************************/
  /*
  *  States
  */
  /*************************************************************/

  /**
  *  @brief Is screen visible?
  */
  bool visible = true;

  /**
  *  @brief Is fullscreen?
  */
  bool fullscreen = false;
};

class SH_CORE_EXPORT Screen
{
 public:
  /**
  *  @brief Default constructor.
  */
  Screen() = default;

  /**
  *  @brief Default destructor.
  */
  ~Screen() = default;

  /**
  *  @brief Init Screen.
  * 
  *  @param ScreenDesc desc
  *  @param SPtr<ScreenEventHandle> eventHandler
  * 
  *  @return bool True if initialized, false if failed.
  */
  bool
  init(ScreenDesc desc, SPtr<ScreenEventHandle> eventHandler);

  /**
  *  @brief Close Screen.
  */
  void
  close();

  /**
  *  @brief Get Screen width.
  * 
  *  @return uint32
  */
  FORCEINLINE uint32
  getWidth();

  /**
  *  @brief Get Screen height.
  * 
  *  @return uint32
  */
  FORCEINLINE uint32
  getHeight();

  /**
  *  @brief Get the Screen handler.
  * 
  *  @return PlatformScreen
  */
  FORCEINLINE PlatformScreen
  getPlatformHandler();

 private:
  /**
  * @brief Screen width.
  */
  uint32 m_width;
  /**
  * @brief Screen height.
  */
  uint32 m_height;

  /**
  *  @brief Screen position in the X axis.
  */
  uint32 m_posX;

  /**
  *  @brief Screen position in the Y axis.
  */
  uint32 m_posY;

  /**
  *  @brief Screen handler.
  */
  PlatformScreen m_screenHandle;
};

FORCEINLINE uint32
Screen::getWidth()
{
  return m_width;
}

FORCEINLINE uint32
Screen::getHeight()
{
  return m_height;
}

FORCEINLINE PlatformScreen
Screen::getPlatformHandler()
{
  return m_screenHandle;
}
}
