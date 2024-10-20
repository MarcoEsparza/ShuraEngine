/*************************************************************/
/*
*  @file    shEvent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief   Event types structures and enums.
*
*  Event types structures and enums.
* 
*  @bug     No bug known
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shEventEnums.h"
#include "shEventData.h"

namespace shEngineSDK {
/**
*  @brief Event class
*/
class SH_CORE_EXPORT Event
{
 public:
  /**
  *  @brief Default constructor.
  */
  Event() = default;
  
  /**
  *  @brief None Event constructor.
  * 
  *  @param shEventType::E _type: Type of event.
  */
  explicit FORCEINLINE Event(const shEventType::E& _type) : type(_type) {}

  /**
  *  @brief Focus Event constructor.
  * 
  *  @param FocusData& _data
  */
  explicit FORCEINLINE Event(const FocusData& _data)
                             : type(shEventType::E::kFocus), data(_data) {}

  /**
  *  @brief Resize Event constructor.
  * 
  *  @param ResizeData& _data
  */
  explicit FORCEINLINE Event(const ResizeData& _data)
                             : type(shEventType::E::kResize), data(_data) {}

  /**
  *  @brief Dpi Event constructor.
  * 
  *  @param DpiData& _data
  */
  explicit FORCEINLINE Event(const DpiData& _data)
                             : type(shEventType::E::kDPI), data(_data) {}

  /**
  *  @brief Keyboard Event constructor.
  * 
  *  @param KeyboardData& _data
  */
  explicit FORCEINLINE Event(const KeyboardData& _data)
                             : type(shEventType::E::kKeyboard), data(_data) {}

  /**
  *  @brief MouseMove Event constructor.
  * 
  *  @param MouseMoveData& _data
  */
  explicit FORCEINLINE Event(const MouseMoveData& _data)
                             : type(shEventType::E::kMouseMove), data(_data) {}

  /**
  *  @brief MouseInput Event constructor.
  * 
  *  @param MouseInputData& _data
  */
  explicit FORCEINLINE Event(const MouseInputData& _data)
                             : type(shEventType::E::kMouseInput), data(_data) {}

  /**
  *  @brief MouseWheel Event constructor.
  * 
  *  @param MouseWheelData& _data
  */
  explicit FORCEINLINE Event(const MouseWheelData& _data)
                             : type(shEventType::E::kMouseWheel), data(_data) {}

  /**
  *  @brief MouseRaw Event constructor.
  * 
  *  @param MouseRawData& _data
  */
  explicit FORCEINLINE Event(const MouseRawData& _data)
                             : type(shEventType::E::kMouseRaw), data(_data) {}

  /**
  *  @brief Default destructor.
  */
  ~Event() = default;

  /*************************************************************/
  /*
  *  Functions
  */
  /*************************************************************/
 public:
  /**
  *  @brief To compare if two event types are equal.
  * 
  *  @param Event& other
  * 
  *  @return bool True if they are equal, false if not.
  */
  FORCEINLINE bool
  equals(const Event& other) const;
  
  /*************************************************************/
  /*
  *  Variables
  */
  /*************************************************************/
 public:
  /**
  *  @brief Union of data.
  */
  EventData data;

  /**
  *  @brief Type of event.
  */
  shEventType::E type = shEventType::E::kNone;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE bool
Event::equals(const Event& other) const
{
  return (type == other.type);
}
}
