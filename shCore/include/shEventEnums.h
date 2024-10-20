/*************************************************************/
/*
*  @file    shEventEnums.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/20
*  @brief   Enumerators for screen events.
*
*  Enumerators for screen events.
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
/**
*  @brief Type of event namespace.
*/
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
*  @brief State of a button press namespace.
*/
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
*  @brief Mouse inputs namespace.
*/
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
*  @brief Key event namespace.
*/
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
}
