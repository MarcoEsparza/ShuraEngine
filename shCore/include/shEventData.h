/*************************************************************/
/*
*  @file    shEventData.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief   Event data types.
*
*  Event data types.
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

namespace shEngineSDK {
/**
*  @brief The state of modifier keys. Pressed is true, released is false.
*/
struct SH_CORE_EXPORT ModifierState
{
  /**
  *  @brief Contructor for Modifier struct, if not given parametars, false is assumed.
  */
  FORCEINLINE ModifierState(const bool _ctrl = false,
    const bool _alt = false,
    const bool _shift = false,
    const bool _meta = false)
    : ctrl(_ctrl), alt(_alt), shift(_shift), meta(_meta) {}

  /**
  *  @brief Control keyboard modifier.
  */
  bool ctrl;

  /**
  *  @brief Alt keyboard modifier.
  */
  bool alt;

  /**
  *  @brief Shift keyboard modifier.
  */
  bool shift;

  /**
  *  @brief Meta keyboard modifier.
  */
  bool meta;
};

/**
*  @brief Data passed with Focus events.
*/
struct SH_CORE_EXPORT FocusData
{
  /**
  *  @brief Struct constructor.
  *
  *  @param bool _focused
  */
  explicit FORCEINLINE FocusData(const bool _focused) : focused(_focused) {}

  /**
  *  @brief Is focused?
  */
  bool focused;
};

/**
*  @brief Data passed with Resize events.
*/
struct SH_CORE_EXPORT ResizeData
{
  /**
  *  @brief Constructor.
  *
  *  @param uint32 _width
  *  @param uint32 _height
  *  @param bool _resizing
  */
  FORCEINLINE ResizeData(const uint32 _width, const uint32 _height, const bool _resizing)
    : width(_width), height(_height), resizing(_resizing) {}

  /**
  *  @brief Screen width.
  */
  uint32 width;

  /**
  *  @brief Screen height.
  */
  uint32 height;

  /**
  *  @brief Is resizing?
  */
  bool resizing;
};

/**
*  @brief Data passed with DPI events.
*/
struct SH_CORE_EXPORT DpiData
{
  /**
  *  @brief Constructor.
  *
  *  @param float _scale
  */
  explicit FORCEINLINE DpiData(const float _scale) : scale(_scale) {}

  /**
  *  @brief Dpi screen scale.
  */
  float scale;
};

/**
*  @brief Structure for keyboard data.
*/
struct SH_CORE_EXPORT KeyboardData
{
  /**
  *  @brief Constructor.
  * 
  *  @param shKey::E _key
  *  @param shButtonState::E _state
  *  @param ModifierState _modifiers
  */
  FORCEINLINE KeyboardData(shKey::E _key, shButtonState::E _state, ModifierState _modifiers)
                           : key(_key), state(_state), modifiers(_modifiers) {}

  /**
  *  @brief Key to interact.
  */
  shKey::E key;

  /**
  *  @brief State of the key.
  */
  shButtonState::E state;

  /**
  *  @brief Keyboard modifiers.
  */
  ModifierState modifiers;
};

/**
*  @brief Structure for raw mouse data
*/
struct SH_CORE_EXPORT MouseRawData
{
  /**
  *  @brief Constructor.
  * 
  *  @param int32 _deltaX
  *  @param int32 _deltaY
  */
  FORCEINLINE MouseRawData(int32 _deltaX,
                           int32 _deltaY)
                           : deltaX(_deltaX),
                             deltaY(_deltaY) {}

  /**
  *  @brief
  */
  int32 deltaX;

  /**
  *  @brief
  */
  int32 deltaY;
};

/**
*  @brief Structure for mouse movement data.
*/
struct SH_CORE_EXPORT MouseMoveData
{
  /**
  *  @brief Constructor.
  * 
  *  @param uint32 _x
  *  @param uint32 _y
  *  @param uint32 _screenX
  *  @param uint32 _screenY
  *  @param int32 _deltaX
  *  @param int32 _deltaY
  */
  FORCEINLINE MouseMoveData(uint32 _x,
                            uint32 _y,
                            uint32 _screenX,
                            uint32 _screenY,
                            int32 _deltaX,
                            int32 _deltaY)
                            : x(_x),
                              y(_y),
                              screenX(_screenX),
                              screenY(_screenY),
                              deltaX(_deltaX),
                              deltaY(_deltaY) {}

  /**
  * @brief Position in window in X axis.
  */
  uint32 x;

  /**
  * @brief Position in window in Y axis.
  */
  uint32 y;

  /**
  * @brief Position in screen in X axis.
  */
  uint32 screenX;

  /**
  * @brief Position in screen in Y axis.
  */
  uint32 screenY;

  /**
  * @brief
  */
  int32 deltaX;

  /**
  * @brief
  */
  int32 deltaY;
};

/**
*  @brief Structure for mouse input data.
*/
struct SH_CORE_EXPORT MouseInputData
{
  /**
  *  @brief Constructor.
  * 
  *  @param shMouseInput::E _button
  *  @param shButtonState::E _state
  *  @param ModifierState _modifiers
  */
  FORCEINLINE MouseInputData(shMouseInput::E _button,
                             shButtonState::E _state,
                             ModifierState _modifiers)
                             : button(_button),
                               state(_state),
                               modifiers(_modifiers) {}

  /**
  *  @brief Button to interact.
  */
  shMouseInput::E button;

  /**
  *  @brief State of button
  */
  shButtonState::E state;

  /**
  *  @brief Keyboard modifiers.
  */
  ModifierState modifiers;
};

/**
*  @brief Structure for mouse wheel data.
*/
struct SH_CORE_EXPORT MouseWheelData
{
  /**
  *  @brief Constructor.
  * 
  *  @param double _delta
  *  @param ModifierState _modifiers
  */
  FORCEINLINE MouseWheelData(double _delta, ModifierState _modifiers)
                             : delta(_delta), modifiers(_modifiers) {}

  /**
  *  @brief Mouse wheel move up or down.
  */
  double delta;

  /**
  *  @brief Keyboard modifiers.
  */
  ModifierState modifiers;
};

/**
* @brief Union of events data.
*/
union SH_CORE_EXPORT EventData
{
  /**
  *  @brief Focus data.
  */
  FocusData focus;

  /**
  *  @brief Resize data.
  */
  ResizeData resize;

  /**
  *  @brief Dpi data.
  */
  DpiData dpi;

  /**
  *  @brief Keyboard data.
  */
  KeyboardData keyboard;

  /**
  *  @brief Mouse move data.
  */
  MouseMoveData mouseMove;

  /**
  *  @brief Mouse input data.
  */
  MouseInputData mouseInput;

  /**
  *  @brief Mouse wheel data.
  */
  MouseWheelData mouseWheel;

  /**
  *  @brief Mouse raw data.
  */
  MouseRawData mouseRaw;

  /**
  *  @brief Constructor.
  */
  FORCEINLINE EventData() {}

  /**
  *  @brief Constructor for FocusData.
  */
  explicit FORCEINLINE EventData(FocusData _focus) : focus(_focus) {}

  /**
  *  @brief Constructor for ResizeData.
  */
  explicit FORCEINLINE EventData(ResizeData _resize) : resize(_resize) {}

  /**
  *  @brief Constructor for DpiData.
  */
  explicit FORCEINLINE EventData(DpiData _dpi) : dpi(_dpi) {}

  /**
  *  @brief Constructor for KeyboardData.
  */
  explicit FORCEINLINE EventData(KeyboardData _keyboard) : keyboard(_keyboard) {}

  /**
  *  @brief Constructor for MouseMoveData.
  */
  explicit FORCEINLINE EventData(MouseMoveData _mouseMove) : mouseMove(_mouseMove) {}

  /**
  *  @brief Constructor for MouseInputData.
  */
  explicit FORCEINLINE EventData(MouseInputData _mouseInput) : mouseInput(_mouseInput) {}

  /**
  *  @brief Constructor for MouseWheelData.
  */
  explicit FORCEINLINE EventData(MouseWheelData _mouseWheel) : mouseWheel(_mouseWheel) {}

  /**
  *  @brief Constructor for MouseRawData.
  */
  explicit FORCEINLINE EventData(MouseRawData _mouseRaw) : mouseRaw(_mouseRaw) {}

  /**
  *  @brief Destructor.
  */
  FORCEINLINE ~EventData() {}
};
}
