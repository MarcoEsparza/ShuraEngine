/*************************************************************/
/*
*  @file    shEvent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/02/20
*  @brief   Event class.
*
*  Event class.
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
  *  @brief Type Event constructor.
  * 
  *  @param shEVENT_TYPE::E _type: Type of event.
  */
  explicit FORCEINLINE Event(const EVENT_TYPE::E& _type) : type(_type) {}

  /**
  *  @brief Focus Event constructor.
  * 
  *  @param bool _focused
  */
  explicit Event(const bool _focused);

  /**
  *  @brief Resize Event constructor.
  * 
  *  @param uint32 _width
  *  @param uint32 _height
  *  @param bool _resizing
  */
  Event(const uint32 _width, const uint32 _height, const bool _resizing);

  /**
  *  @brief Dpi Event constructor.
  * 
  *  @param float _scale
  */
  explicit Event(const float _scale);

  /**
  *  @brief Keyboard Event constructor.
  * 
  *  @param shKEY::E _key
  *  @param shBUTTON_STATE::E _state
  *  @param ModifierState& _modifiers
  */
  Event(const KEY::E _key,
        const BUTTON_STATE::E _state,
        const ModifierState& _modifiers);

  /**
  *  @brief MouseMove Event constructor.
  * 
  *  @param uint32 _x
  *  @param uint32 _y
  *  @param uint32 _screenX
  *  @param uint32 _screenY
  *  @param int32 _deltaX
  *  @param int32 _deltaY
  */
  Event(const uint32 _x,
        const uint32 _y,
        const uint32 _screenX,
        const uint32 _screenY,
        const int32 _deltaX,
        const int32 _deltaY);

  /**
  *  @brief MouseInput Event constructor.
  * 
  *  @param shMOUSE_INPUT::E _button
  *  @param shBUTTON_STATE::E _state
  *  @param ModifierState& _modifiers
  */
  Event(const MOUSE_INPUT::E _button,
        const BUTTON_STATE::E _state,
        const ModifierState& _modifiers);

  /**
  *  @brief MouseWheel Event constructor.
  * 
  *  @param double _delta
  *  @param ModifierState& _modifiers
  */
  Event(const double _delta, const ModifierState& _modifiers);

  /**
  *  @brief MouseHorizontalWheel Event constructor.
  *
  *  @param double _delta
  *  @param ModifierState& _modifiers
  *  @param const bool _used
  */
  Event(const double _delta, const ModifierState& _modifiers, const bool _used);

  /**
  *  @brief MouseRaw Event constructor.
  * 
  *  @param int32 _deltaX
  *  @param int32 _deltaY
  */
  Event(const int32 _deltaX, const int32 _deltaY);

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
  typeEquals(const Event& other) const;
  
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
  EVENT_TYPE::E type = EVENT_TYPE::kNone;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE bool
Event::typeEquals(const Event& other) const
{
  return (type == other.type);
}
}
