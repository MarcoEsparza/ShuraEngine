/*************************************************************/
/*
*  @file    shEvent.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/08
*  @brief
*
*
*
*  @bug
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shEvent.h"

namespace shEngineSDK {
/*************************************************************/
/*
*  Static variables
*/
/*************************************************************/
const shEventType::E FocusData::type = shEventType::E::kFocus;

const shEventType::E ResizeData::type = shEventType::E::kResize;

const shEventType::E DpiData::type = shEventType::E::kDPI;

const shEventType::E KeyboardData::type = shEventType::E::kKeyboard;

const shEventType::E MouseRawData::type = shEventType::E::kMouseRaw;

const shEventType::E MouseMoveData::type = shEventType::E::kMouseMove;

const shEventType::E MouseInputData::type = shEventType::E::kMouseInput;

const shEventType::E MouseWheelData::type = shEventType::E::kMouseWheel;

const KeyConvertion::KeyToCharMap KeyConvertion::sKeyToCharMap = {
      "\x1B", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "\b",
      "\t", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "\r",
      "", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", ":", "'", "\"",
      "`", "", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "",
      "*", "", " ", "", "", "", "", "", "", "", "", "", "", "",
      "", "", "7", "8", "9", "-", "4", "5", "6", "+", "1", "2", "3", "0",
      ".", "", "", "\r", "", "/", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", ""};

const UMap<const char*, shKey::E> KeyConvertion::sCharToKeyMap = {
  {"\x1B", shKey::E::kEscape},
  {"1", shKey::E::kNum1},
  {"2", shKey::E::kNum2},
  {"3", shKey::E::kNum3},
  {"4", shKey::E::kNum4},
  {"5", shKey::E::kNum5},
  {"6", shKey::E::kNum6},
  {"7", shKey::E::kNum7},
  {"8", shKey::E::kNum8},
  {"9", shKey::E::kNum9},
  {"0", shKey::E::kNum0},
  {"-", shKey::E::kSubstract},
  {"=", shKey::E::kEquals},
  {"\b", shKey::E::kBack},
  {"\t", shKey::E::kTab},
  {"Q", shKey::E::kQ},
  {"W", shKey::E::kW},
  {"E", shKey::E::kE},
  {"R", shKey::E::kR},
  {"T", shKey::E::kT},
  {"Y", shKey::E::kY},
  {"U", shKey::E::kU},
  {"I", shKey::E::kI},
  {"O", shKey::E::kO},
  {"P", shKey::E::kP},
  {"[", shKey::E::kLBracket},
  {"]", shKey::E::kRBracket},
  {"\r", shKey::E::kEnter},
  {"A", shKey::E::kA},
  {"S", shKey::E::kS},
  {"D", shKey::E::kD},
  {"F", shKey::E::kF},
  {"G", shKey::E::kG},
  {"H", shKey::E::kH},
  {"J", shKey::E::kJ},
  {"K", shKey::E::kK},
  {"L", shKey::E::kL},
  {";", shKey::E::kSemicolon},
  {":", shKey::E::kColon},
  {"'", shKey::E::kApostrophe},
  {"\"", shKey::E::kSemicolon},
  {"`", shKey::E::kGrave},
  {"Left Shift", shKey::E::kLShift},
  {"\\", shKey::E::kBackslash},
  {"Z", shKey::E::kZ},
  {"X", shKey::E::kX},
  {"C", shKey::E::kC},
  {"V", shKey::E::kV},
  {"B", shKey::E::kB},
  {"N", shKey::E::kN},
  {"M", shKey::E::kM},
  {",", shKey::E::kComma},
  {".", shKey::E::kPeriod},
  {"/", shKey::E::kSlash},
  {"Right Shift", shKey::E::kRShift},
  {"*", shKey::E::kMultiply},
  {"Left Alt", shKey::E::kLAlt},
  {" ", shKey::E::kSpace},
  {"Capital", shKey::E::kCapital},
  {"F1", shKey::E::kF1},
  {"F2", shKey::E::kF2},
  {"F3", shKey::E::kF3},
  {"F4", shKey::E::kF4},
  {"F5", shKey::E::kF5},
  {"F6", shKey::E::kF6},
  {"F7", shKey::E::kF7},
  {"F8", shKey::E::kF8},
  {"F9", shKey::E::kF9},
  {"F10", shKey::E::kF10},
  {"NumLock", shKey::E::kNumLock}
};

/*************************************************************/
/*
*  Constructors
*/
/*************************************************************/

Event::Event(shEventType::E _type, SPtr<Screen> _screen)
{
  screen = _screen;
  type = _type;
}

Event::Event(FocusData _data, WPtr<Screen> _screen)
{
  auto sc = _screen.lock();
  screen = sc;
  type = shEventType::E::kFocus;
  data.focus = _data;
}

Event::Event(ResizeData _data, WPtr<Screen> _screen)
{
  auto sc = _screen.lock();
  screen = sc;
  type = shEventType::E::kResize;
  data.resize = _data;
}

Event::Event(DpiData _data, WPtr<Screen> _screen)
{
  auto sc = _screen.lock();
  screen = sc;
  type = shEventType::E::kDPI;
  data.dpi = _data;
}

Event::Event(KeyboardData _data, WPtr<Screen> _screen)
{
  auto sc = _screen.lock();
  screen = sc;
  type = shEventType::E::kKeyboard;
  data.keyboard = _data;
}

Event::Event(MouseMoveData _data, WPtr<Screen> _screen)
{
  auto sc = _screen.lock();
  screen = sc;
  type = shEventType::E::kMouseMove;
  data.mouseMove = _data;
}

Event::Event(MouseInputData _data, WPtr<Screen> _screen)
{
  auto sc = _screen.lock();
  screen = sc;
  type = shEventType::E::kMouseInput;
  data.mouseInput = _data;
}

Event::Event(MouseWheelData _data, WPtr<Screen> _screen)
{
  auto sc = _screen.lock();
  screen = sc;
  type = shEventType::E::kMouseWheel;
  data.mouseWheel = _data;
}

Event::Event(MouseRawData _data, WPtr<Screen> _screen)
{
  auto sc = _screen.lock();
  screen = sc;
  type = shEventType::E::kMouseRaw;
  data.mouseRaw = _data;
}

/*************************************************************/
/*
*  Functions
*/
/*************************************************************/

String
KeyConvertion::convertKeyToString(const shKey::E key)
{
  return sKeyToCharMap[static_cast<SIZE_T>(key)];
}

shKey::E
KeyConvertion::consvertStringToKey(const String& str)
{
  shKey::E k = shKey::E::kKeysMax;

  auto itr = sCharToKeyMap.find(str.c_str());

  if (itr != sCharToKeyMap.end()) {
    k = (*itr).second;
  }

  return k;
}
}
