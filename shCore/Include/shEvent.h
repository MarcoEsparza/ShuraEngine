/*************************************************************/
/*
*  @file    shEvent.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/19
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

namespace shEngineSDK {
namespace shEventType {
/**
*  @brief Type of event enumeration.
*/
enum SH_CORE_EXPORT E
{
  kNone = 0,
  kClose,
  kCreate,
  kFocus,
  kPaint,
  kResize,
  kDPI,
  kKeyboard,
  kMouseMove,
  kMouseRaw,
  kMouseWheel,
  kMouseInput,
  kDropFile,
  kHoverFile,
  kEventTypeMax
};
}

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
  FORCEINLINE explicit FocusData(const bool _focused) : focused(_focused) {}

  /**
  *  @brief Is focused?
  */
  bool focused;

  /**
  *  @brief Type of event.
  */
  static const shEventType::E type;
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

  /**
  *  @brief Type of event.
  */
  static const shEventType::E type;
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
  FORCEINLINE explicit DpiData(const float _scale) : scale(_scale) {}
  
  /**
  *  @brief Dpi screen scale.
  */
  float scale;

  /**
  *  @brief Type of event.
  */
  static const shEventType::E type;
};

namespace shButtonState {
/**
*  @brief State of a button press enumeration.
*/
enum SH_CORE_EXPORT E
{
  kPressed = 0,
  kReleased,
  kButtonStateMax
};
}

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

namespace shKey {
/**
*  @brief Key event enumeration.
*/
enum SH_CORE_EXPORT E
{
  kEscape = 0,
  kNum1,
  kNum2,
  kNum3,
  kNum4,
  kNum5,
  kNum6,
  kNum7,
  kNum8,
  kNum9,
  kNum0,
  kMinus,
  kEquals,
  kBack,
  kTab,
  kQ,
  kW,
  kE,
  kR,
  kT,
  kY,
  kU,
  kI,
  kO,
  kP,
  kLBracket,
  kRBracket,
  kEnter,
  kLControl,
  kA,
  kS,
  kD,
  kF,
  kG,
  kH,
  kJ,
  kK,
  kL,
  kSemicolon,
  kColon,
  kApostrophe,
  kQuotation,
  kGrave,
  kLShift,
  kBackslash,
  kZ,
  kX,
  kC,
  kV,
  kB,
  kN,
  kM,
  kComma,
  kPeriod,
  kSlash,
  kRShift,
  kMultiply,
  kLAlt,
  kSpace,
  kCapital,
  kF1,
  kF2,
  kF3,
  kF4,
  kF5,
  kF6,
  kF7,
  kF8,
  kF9,
  kF10,
  kNumLock,
  kScroll,
  kNumpad7,
  kNumpad8,
  kNumpad9,
  kSubstract,
  kNumpad4,
  kNumpad5,
  kNumpad6,
  kAdd,
  kNumpad1,
  kNumpad2,
  kNumpad3,
  kNumpad0,
  kDecimal,
  kF11,
  kF12,
  kNumpadEnter,
  kRControl,
  kDivide,
  kSysrq,
  kRAlt,
  kPause,
  kHome,
  kUp,
  kPgUp,
  kLeft,
  kRight,
  kEnd,
  kDown,
  kPgDn,
  kInsert,
  kDel,
  kLWin,
  kRWin,
  kApps,

  kKeysMax
};
}

class SH_CORE_EXPORT KeyConvertion
{
 public:
  KeyConvertion() = default;
  ~KeyConvertion() = default;

  /**
  *  @brief Type for elements in a byte.
  */
  typedef const char charElm[256];

  /**
  *  @brief Type to convert from key to char map.
  */
  typedef const charElm KeyToCharMap[static_cast<SIZE_T>(shKey::E::kKeysMax)];

  /**
  *  @brief Type to convert char to key map.
  */
  typedef shKey::E CharToKeyMap[static_cast<SIZE_T>(shKey::E::kKeysMax)];

  /**
  *  @brief Variable that contains char map.
  */
  static const KeyToCharMap sKeyToCharMap;

  /**
  *  @brief Unordered map to access key through string.
  */
  static const UMap<const char*, shKey::E> sCharToKeyMap;

  /**
  *  @brief Function to convert keys to string.
  *
  *  @param shKey::E key
  *
  *  @return String
  */
  String convertKeyToString(const shKey::E& key);

  /**
  *  @brief Function to convert string to key.
  *
  *  @param String& str
  *
  *  @return shKey::E
  */
  shKey::E consvertStringToKey(const String& str);
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

  /**
  *  @brief Type of event.
  */
  static const shEventType::E type;
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

  /**
  *  @brief Type of event.
  */
  static const shEventType::E type;
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

  /**
  * @brief Type of event.
  */
  static const shEventType::E type;
};

namespace shMouseInput {
/**
*  @brief Mouse inputs enumeration.
*/
enum SH_CORE_EXPORT E
{
  kLeft = 0,
  kRight,
  kMiddle,
  kButton4,
  kButton5,
  kMouseInputMax
};
}

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

  /**
  *  @brief Type of event.
  */
  static const shEventType::E type;
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

  /**
  *  @brief Type of event.
  */
  static const shEventType::E type;
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
  *  @brief Destructor.
  */
  FORCEINLINE ~EventData() {}
};

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
  explicit Event(const shEventType::E& _type) : type(_type) {}

  /**
  *  @brief Focus Event constructor.
  * 
  *  @param shEventType::E _type: Type of event.
  */
  explicit Event(const FocusData& _data);

  /**
  *  @brief Resize Event constructor.
  * 
  *  @param shEventType::E _type: Type of event.
  */
  explicit Event(const ResizeData& _data);

  /**
  *  @brief Dpi Event constructor.
  * 
  *  @param shEventType::E _type: Type of event.
  */
  explicit Event(const DpiData& _data);

  /**
  *  @brief Keyboard Event constructor.
  * 
  *  @param shEventType::E _type: Type of event.
  */
  explicit Event(const KeyboardData& _data);

  /**
  *  @brief MouseMove Event constructor.
  * 
  *  @param shEventType::E _type: Type of event.
  */
  explicit Event(const MouseMoveData& _data);

  /**
  *  @brief MouseInput Event constructor.
  * 
  *  @param shEventType::E _type: Type of event.
  */
  explicit Event(const MouseInputData& _data);

  /**
  *  @brief MouseWheel Event constructor.
  * 
  *  @param shEventType::E _type: Type of event.
  */
  explicit Event(const MouseWheelData& _data);

  /**
  *  @brief MouseRaw Event constructor.
  * 
  *  @param shEventType::E _type: Type of event.
  */
  explicit Event(const MouseRawData& _data);

  /**
  *  @brief Default destructor.
  */
  ~Event() = default;

  /*************************************************************/
  /*
  *  Operator overload
  */
  /*************************************************************/
 public:
  /**
  *  @brief Operator to compare if two events are equal.
  * 
  *  @param lValue-Event
  *  @param rValue-Event
  * 
  *  @return bool True if they are equal, false if not.
  */
  FORCEINLINE bool
  operator==(const Event& other) const;
  
  /*************************************************************/
  /*
  *  Varables
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
  shEventType::E type;
};

/*************************************************************/
/*
*  Implementations
*/
/*************************************************************/

FORCEINLINE bool
Event::operator==(const Event& other) const
{
  return type == other.type;
}
}
