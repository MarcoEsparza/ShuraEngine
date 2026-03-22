/*****************************************************************************/
/*
*  @file    shScreenEventHandleX11.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/19
*  @brief   Base screen event handler for Linux.
*
*  Base screen event handler for Linux.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shScreenEventHandle.h"
#include <shVector2i.h>

#if SH_PLATFORM == SH_PLATFORM_LINUX

#include <X11/Xlib.h>
#include <X11/keysym.h>

namespace shEngineSDK {
void
pushEvent(const XEvent& xEv, WPtr<Screen> screen, Queue<Event>& eventQueue);

void
ScreenEventHandle::update() {
  XEvent event;
  if (m_parentScreen.expired()) {
      return;
  }
  auto parentScreenPtr = m_parentScreen.lock();

  auto display = parentScreenPtr->getPlatformHandler()->display;

  while (XPending(display) > 0) {
    XNextEvent(display, &event);
    pushEvent(event, parentScreenPtr, m_queue);
  }
}

bool
ScreenEventHandle::empty() const {
  return m_queue.empty();
}

const Event&
ScreenEventHandle::front() const {
  return m_queue.front();
}

void
ScreenEventHandle::pop() {
  m_queue.pop();
}

void
ScreenEventHandle::emplace(const Event& ev)
{
  if (ev.type == EVENT_TYPE::kFocus) {
    m_queue.emplace(ev.data.focus.focused);
  }
  else if (ev.type == EVENT_TYPE::kResize) {
    m_queue.emplace(ev.data.resize.width,
                    ev.data.resize.height,
                    ev.data.resize.resizing);
  }
  else if (ev.type == EVENT_TYPE::kDPI) {
    m_queue.emplace(ev.data.dpi.scale);
  }
  else if (ev.type == EVENT_TYPE::kKeyboard) {
    m_queue.emplace(ev.data.keyboard.key,
                    ev.data.keyboard.state,
                    ev.data.keyboard.modifiers);
  }
  else if (ev.type == EVENT_TYPE::kMouseMove) {
    m_queue.emplace(ev.data.mouseMove.x,
                    ev.data.mouseMove.y,
                    ev.data.mouseMove.screenX,
                    ev.data.mouseMove.screenY,
                    ev.data.mouseMove.deltaX,
                    ev.data.mouseMove.deltaY);
  }
  else if (ev.type == EVENT_TYPE::kMouseInput) {
    m_queue.emplace(ev.data.mouseInput.button,
                    ev.data.mouseInput.state,
                    ev.data.mouseInput.modifiers);
  }
  else if (ev.type == EVENT_TYPE::kMouseWheel) {
    m_queue.emplace(ev.data.mouseWheel.delta,
                    ev.data.mouseWheel.modifiers);
  }
  else if (ev.type == EVENT_TYPE::kMouseHWheel) {
    m_queue.emplace(ev.data.mouseWheel.delta,
                    ev.data.mouseWheel.modifiers,
                    true);
  }
  else if (ev.type == EVENT_TYPE::kMouseRaw) {
    m_queue.emplace(ev.data.mouseRaw.deltaX,
                    ev.data.mouseRaw.deltaY);
  }
  else {
    m_queue.emplace(ev.type);
  }
}

void
ScreenEventHandle::setProcessingMode(const shPROCESSING_MODE::E& mode)
{
  m_processingMode = mode;
}

void
ScreenEventHandle::setParentScreen(const WPtr<Screen>& parentScreen)
{
  m_parentScreen = parentScreen;
}

SIZE_T
ScreenEventHandle::getSize() const
{
  return m_queue.size();
}

void
pushEvent(const XEvent& event, WPtr<Screen> screen, Queue<Event>& eventQueue) {
  auto handle = screen->getPlatformHandler();
  auto pScreen = screen.lock();
  Event currentEvent;
  switch (event.type) {
    case FocusIn:
      currentEvent = Event(true);
      break;
    case FocusOut:
      currentEvent = Event(false);
      break;
    case ConfigureNotify:
      currentEvent = Event(event.xconfigure.width, event.xconfigure.height, false);
      break;
    case Expose:
      //currentEvent = Event(EVENT_TYPE::kRedraw);
      break;
    case MapNotify:
      //currentEvent = Event(EVENT_TYPE::kShow);
      break;
    case UnmapNotify:
      //currentEvent = Event(EVENT_TYPE::kHide);
      break; 
    case ClientMessage:
      if((Atom)event.xclient.data.1[0] == handle->wDelete){
        currentEvent = Event(EVENT_TYPE::kClose);
      }
      break;
    case ButtonPress:
    case ButtonRelease:
    {
      XButtonEvent& buttonEvent = event.xbutton;

      MOUSE_BUTTON::E button;
      switch (buttonEvent.button) {
        case Button1: button = MOUSE_BUTTON::E::kLeft; break;
        case Button2: button = MOUSE_BUTTON::E::kMiddle; break;
        case Button3: button = MOUSE_BUTTON::E::kRight; break;
        case Button4: button = MOUSE_BUTTON::E::kWheelUp; break;
        case Button5: button = MOUSE_BUTTON::E::kWheelDown; break;
        case 6: button = MOUSE_BUTTON::E::kWheelLeft; break;
        case 7: button = MOUSE_BUTTON::E::kWheelRight; break;
        default: button = MOUSE_BUTTON::E::kButtonsMax; break;
      }

      // 🔹 Modifier state
      ModifierState ms;
      ms.shift = buttonEvent.state & ShiftMask;
      ms.ctrl  = buttonEvent.state & ControlMask;
      ms.alt   = buttonEvent.state & Mod1Mask;
      ms.meta  = buttonEvent.state & Mod4Mask;

      // 🔹 Event
      if (event.type == ButtonPress) {
        currentEvent = Event(button, BUTTON_STATE::kPressed, ms);
      } else {
        currentEvent = Event(button, BUTTON_STATE::kReleased, ms);
      }

      break;
    }
    case MotionNotify:
    {
      XMotionEvent& motionEvent = event.xmotion;

      // 🔹 Modifier state
      ModifierState ms;
      ms.shift = motionEvent.state & ShiftMask;
      ms.ctrl  = motionEvent.state & ControlMask;
      ms.alt   = motionEvent.state & Mod1Mask;
      ms.meta  = motionEvent.state & Mod4Mask;

      uint32 deltaX = motionEvent.x - pScreen->getPrevMousePos().x;
      uint32 deltaY = motionEvent.y - pScreen->getPrevMousePos().y

      currentEvent = Event(motionEvent.x, motionEvent.y,
                           motionEvent.x_root, motionEvent.y_root,
                           deltaX, deltaY, ms);

      pScreen->setPrevMousePos(Vector2i(motionEvent.x, motionEvent.y));
      break;
    }
    case KeyPress:
    case KeyRelease:
    {
    XKeyEvent& keyEvent = event.xkey;

    KeySym sym = XLookupKeysym(&keyEvent, 0);

    KEY::E key;

    switch (sym)
    {
    case XK_Escape: key = KEY::E::kEscape; break;

    case XK_0: key = KEY::E::kNum0; break;
    case XK_1: key = KEY::E::kNum1; break;
    case XK_2: key = KEY::E::kNum2; break;
    case XK_3: key = KEY::E::kNum3; break;
    case XK_4: key = KEY::E::kNum4; break;
    case XK_5: key = KEY::E::kNum5; break;
    case XK_6: key = KEY::E::kNum6; break;
    case XK_7: key = KEY::E::kNum7; break;
    case XK_8: key = KEY::E::kNum8; break;
    case XK_9: key = KEY::E::kNum9; break;

    case XK_a: case XK_A: key = KEY::E::kA; break;
    case XK_b: case XK_B: key = KEY::E::kB; break;
    case XK_c: case XK_C: key = KEY::E::kC; break;
    case XK_d: case XK_D: key = KEY::E::kD; break;
    case XK_e: case XK_E: key = KEY::E::kE; break;
    case XK_f: case XK_F: key = KEY::E::kF; break;
    case XK_g: case XK_G: key = KEY::E::kG; break;
    case XK_h: case XK_H: key = KEY::E::kH; break;
    case XK_i: case XK_I: key = KEY::E::kI; break;
    case XK_j: case XK_J: key = KEY::E::kJ; break;
    case XK_k: case XK_K: key = KEY::E::kK; break;
    case XK_l: case XK_L: key = KEY::E::kL; break;
    case XK_m: case XK_M: key = KEY::E::kM; break;
    case XK_n: case XK_N: key = KEY::E::kN; break;
    case XK_o: case XK_O: key = KEY::E::kO; break;
    case XK_p: case XK_P: key = KEY::E::kP; break;
    case XK_q: case XK_Q: key = KEY::E::kQ; break;
    case XK_r: case XK_R: key = KEY::E::kR; break;
    case XK_s: case XK_S: key = KEY::E::kS; break;
    case XK_t: case XK_T: key = KEY::E::kT; break;
    case XK_u: case XK_U: key = KEY::E::kU; break;
    case XK_v: case XK_V: key = KEY::E::kV; break;
    case XK_w: case XK_W: key = KEY::E::kW; break;
    case XK_x: case XK_X: key = KEY::E::kX; break;
    case XK_y: case XK_Y: key = KEY::E::kY; break;
    case XK_z: case XK_Z: key = KEY::E::kZ; break;

    case XK_minus: key = KEY::E::kMinus; break;
    case XK_plus:  key = KEY::E::kAdd; break;
    case XK_asterisk: key = KEY::E::kMultiply; break;
    case XK_slash: key = KEY::E::kDivide; break;

    case XK_BackSpace: key = KEY::E::kBack; break;
    case XK_Return: key = KEY::E::kEnter; break;
    case XK_Delete: key = KEY::E::kDel; break;
    case XK_Tab: key = KEY::E::kTab; break;

    case XK_Up: key = KEY::E::kUp; break;
    case XK_Down: key = KEY::E::kDown; break;
    case XK_Left: key = KEY::E::kLeft; break;
    case XK_Right: key = KEY::E::kRight; break;

    case XK_space: key = KEY::E::kSpace; break;
    case XK_Home: key = KEY::E::kHome; break;

    case XK_F1: key = KEY::E::kF1; break;
    case XK_F2: key = KEY::E::kF2; break;
    case XK_F3: key = KEY::E::kF3; break;
    case XK_F4: key = KEY::E::kF4; break;
    case XK_F5: key = KEY::E::kF5; break;
    case XK_F6: key = KEY::E::kF6; break;
    case XK_F7: key = KEY::E::kF7; break;
    case XK_F8: key = KEY::E::kF8; break;
    case XK_F9: key = KEY::E::kF9; break;
    case XK_F10: key = KEY::E::kF10; break;
    case XK_F11: key = KEY::E::kF11; break;
    case XK_F12: key = KEY::E::kF12; break;

    case XK_Shift_L: key = KEY::E::kLShift; break;
    case XK_Shift_R: key = KEY::E::kRShift; break;
    case XK_Control_L: key = KEY::E::kLControl; break;
    case XK_Control_R: key = KEY::E::kRControl; break;
    case XK_Alt_L: key = KEY::E::kLAlt; break;
    case XK_Alt_R: key = KEY::E::kRAlt; break;
    case XK_Super_L: key = KEY::E::kLWin; break;
    case XK_Super_R: key = KEY::E::kRWin; break;

    case XK_period: key = KEY::E::kPeriod; break;
    case XK_comma: key = KEY::E::kComma; break;
    case XK_semicolon: key = KEY::E::kSemicolon; break;
    case XK_backslash: key = KEY::E::kBackslash; break;
    case XK_grave: key = KEY::E::kGrave; break;
    case XK_bracketleft: key = KEY::E::kLBracket; break;
    case XK_bracketright: key = KEY::E::kRBracket; break;
    case XK_apostrophe: key = KEY::E::kApostrophe; break;

    default:
        key = KEY::E::kKeysMax;
    }

    // 🔹 Modifier state
    ModifierState ms;
    ms.shift = keyEvent.state & ShiftMask;
    ms.ctrl  = keyEvent.state & ControlMask;
    ms.alt   = keyEvent.state & Mod1Mask;
    ms.meta  = keyEvent.state & Mod4Mask;

    // 🔹 Event
    if (event.type == KeyPress)
    {
      currentEvent = Event(key, BUTTON_STATE::kPressed, ms);
    }
    else
    {
      currentEvent = Event(key, BUTTON_STATE::kReleased, ms);
    }

    break;
    }
    // Additional event mappings can be added here
    default:
      return; // Unhandled event type
  }
  
  if(currentEvent.type != EVENT_TYPE::kNone) {
    eventQueue.emplace(currentEvent);
  }
}
}

#endif
