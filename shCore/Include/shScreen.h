/*************************************************************/
/*
*  @file    shScreen.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/23
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
#include "shVector2i.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32
struct HWND__;
using PlatformScreen = HWND__*;
#elif SH_PLATFORM == SH_PLATFORM_LINUX
using PlatformScreen = void*;
#endif

namespace shEngineSDK {
/**
*  @brief Structure for screen description.
*/
struct SH_CORE_EXPORT ScreenDesc
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
  init(const ScreenDesc& desc, const SPtr<ScreenEventHandle>& eventHandler);

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
  getWidth() const;

  /**
  *  @brief Get Screen height.
  * 
  *  @return uint32
  */
  FORCEINLINE uint32
  getHeight() const;


  /**
  *  @brief Return the previous mouse position in X axis.
  *
  *  @return uint32
  */
  FORCEINLINE Vector2i
  getPreviousMousePos() const;

  /**
  *  @brief Get the Screen handler.
  * 
  *  @return PlatformScreen
  */
  FORCEINLINE PlatformScreen
  getPlatformHandler() const;

 private:
  /**
  * @brief Screen width.
  */
  uint32 m_width = 0;
  /**
  * @brief Screen height.
  */
  uint32 m_height = 0;

  /**
  *  @brief Screen position in the X axis.
  */
  uint32 m_posX = 0;

  /**
  *  @brief Screen position in the Y axis.
  */
  uint32 m_posY = 0;

  /**
  *  @brief Previous mouse position.
  */
  Vector2i m_prevMousePos;

  /**
  *  @brief Screen handler.
  */
  PlatformScreen m_screenHandle = nullptr;

  /**
  *  @brief EventQueue
  */
  SPtr<ScreenEventHandle> m_eventQueue;
};

FORCEINLINE uint32
Screen::getWidth() const
{
  return m_width;
}

FORCEINLINE uint32
Screen::getHeight() const
{
  return m_height;
}

FORCEINLINE PlatformScreen
Screen::getPlatformHandler() const
{
  return m_screenHandle;
}

FORCEINLINE Vector2i
Screen::getPreviousMousePos() const
{
  return m_prevMousePos;
}
}
