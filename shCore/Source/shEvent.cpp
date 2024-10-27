/*************************************************************/
/*
*  @file    shEvent.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/23
*  @brief   Event class.
*
*  Event class.
*
*  @bug     No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shEvent.h"

namespace shEngineSDK {
Event::Event(const bool _focused)
{
  type = shEVENT_TYPE::kFocus;
  data.focus = FocusData(_focused);
}

Event::Event(const uint32 _width, const uint32 _height, const bool _resizing)
{
  type = shEVENT_TYPE::kResize;
  data.resize = ResizeData(_width, _height, _resizing);
}

Event::Event(const float _scale)
{
  type = shEVENT_TYPE::kDPI;
  data.dpi = DpiData(_scale);
}

Event::Event(const shKEY::E _key,
             const shBUTTON_STATE::E _state,
             const ModifierState& _modifiers)
{
  type = shEVENT_TYPE::kKeyboard;
  data.keyboard = KeyboardData(_key, _state, _modifiers);
}

Event::Event(const uint32 _x,
             const uint32 _y,
             const uint32 _screenX,
             const uint32 _screenY,
             const int32 _deltaX,
             const int32 _deltaY)
{
  type = shEVENT_TYPE::kMouseMove;
  data.mouseMove = MouseMoveData(_x, _y, _screenX, _screenY, _deltaX, _deltaY);
}

Event::Event(const shMOUSE_INPUT::E _button,
             const shBUTTON_STATE::E _state,
             const ModifierState& _modifiers)
{
  type = shEVENT_TYPE::kMouseInput;
  data.mouseInput = MouseInputData(_button, _state, _modifiers);
}

Event::Event(const double _delta, const ModifierState& _modifiers)
{
  type = shEVENT_TYPE::kMouseWheel;
  data.mouseWheel = MouseWheelData(_delta, _modifiers);
}

Event::Event(const int32 _deltaX, const int32 _deltaY)
{
  type = shEVENT_TYPE::kMouseRaw;
  data.mouseRaw = MouseRawData(_deltaX, _deltaY);
}
}
