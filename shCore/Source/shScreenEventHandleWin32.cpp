/*************************************************************/
/*
*  @file    shScreenEventHandle.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/17
*  @brief   Base screen event handler for windows.
*
*  Base screen event handler for windows.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shScreenEventHandle.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32

#include "Windows.h"

namespace shEngineSDK {
void
ScreenEventHandle::update()
{
  MSG msg = { };

  for (;;) {
    if (m_processingMode == shProcessingMode::E::kPoll) {
      if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        break;
      }
    }
    else {
      GetMessage(&msg, NULL, 0, 0);
    }

    if (msg.message == WM_QUIT) {
      return;
    }

    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

Queue<Event>
ScreenEventHandle::getQueue()
{
  return m_queue;
}

bool
ScreenEventHandle::empty()
{
  return m_queue.empty();
}

Event
ScreenEventHandle::front()
{
  return m_queue.front();
}

void
ScreenEventHandle::pop()
{
  m_queue.pop();
}

void
ScreenEventHandle::setProcessingNode(shProcessingMode::E mode)
{
  m_processingMode = mode;
}

SIZE_T
ScreenEventHandle::getSize()
{
  return m_queue.size();
}

uint32
ScreenEventHandle::getPreviousMouseX() const
{
  return m_prevMouseX;
}

uint32
ScreenEventHandle::getPreviousMouseY() const
{
  return m_prevMouseY;
}

void
ScreenEventHandle::setPreviousMouseX(uint32 pos)
{
  m_prevMouseX = pos;
}

void
ScreenEventHandle::setPreviousMouseY(uint32 pos)
{
  m_prevMouseY = pos;
}

LONG_PTR CALLBACK
windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  LRESULT result = 0;
  RECT currenScreenRect = { -1,-1,-1,-1 };

  Event ev = Event(shEventType::E::kNone);

  ScreenEventHandle* eventQ = reinterpret_cast<ScreenEventHandle*>(GetWindowLongPtr(hwnd, 0));

  switch (msg) {
  case WM_CREATE:
  {
    ev = Event(shEventType::E::kCreate);
    break;
  }
  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    RECT rect;
    GetWindowRect(hwnd, &rect);
    int32 cxWidth = rect.right - rect.left;
    int32 cyHeight = rect.bottom - rect.top;
    HBRUSH borderBrush = CreateSolidBrush(RGB(0, 0, 0));
    rect.left = 0;
    rect.top = 0;
    rect.right = cxWidth;
    rect.bottom = cyHeight;
    FillRect(ps.hdc, &rect, borderBrush);
    EndPaint(hwnd, &ps);

    ev = Event(shEventType::E::kPaint);
    break;
  }
  case WM_ERASEBKGND:
  {
    break;
  }
  case WM_CLOSE:
  case WM_DESTROY:
  {
    ev = Event(shEventType::E::kClose);
    break;
  }
  case WM_SETFOCUS:
  {
    ev = Event(FocusData(true));
    break;
  }
  case WM_KILLFOCUS:
  {
    ev = Event(FocusData(true));
    break;
  }
  case WM_MOUSEWHEEL:
  {
    short modifiers = LOWORD(wParam);
    ev = Event(MouseWheelData(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA,
                              ModifierState(modifiers & MK_CONTROL,
                                            modifiers & MK_ALT,
                                            modifiers & MK_SHIFT,
                                            modifiers & 0)));
    break;
  }
  case WM_LBUTTONDOWN:
  {
    short modifiers = LOWORD(wParam);
    ev = Event(MouseInputData(shMouseInput::E::kLeft,
                              shButtonState::E::kPressed,
                              ModifierState(modifiers & MK_CONTROL,
                                            modifiers & MK_ALT,
                                            modifiers & MK_SHIFT,
                                            modifiers & 0)));
    break;
  }
  case WM_LBUTTONUP:
  {
    short modifiers = LOWORD(wParam);
    ev = Event(MouseInputData(shMouseInput::E::kLeft,
                              shButtonState::E::kReleased,
                              ModifierState(modifiers & MK_CONTROL,
                                            modifiers & MK_ALT,
                                            modifiers & MK_SHIFT,
                                            modifiers & 0)));
    break;
  }
  case WM_MBUTTONDOWN:
  {
    short modifiers = LOWORD(wParam);
    ev = Event(MouseInputData(shMouseInput::E::kMiddle,
                              shButtonState::E::kPressed,
                              ModifierState(modifiers & MK_CONTROL,
                                            modifiers & MK_ALT,
                                            modifiers & MK_SHIFT,
                                            modifiers & 0)));
    break;
  }
  case WM_MBUTTONUP:
  {
    short modifiers = LOWORD(wParam);
    ev = Event(MouseInputData(shMouseInput::E::kMiddle,
                              shButtonState::E::kReleased,
                              ModifierState(modifiers & MK_CONTROL,
                                            modifiers & MK_ALT,
                                            modifiers & MK_SHIFT,
                                            modifiers & 0)));
    break;
  }
  case WM_RBUTTONDOWN:
  {
    short modifiers = LOWORD(wParam);
    ev = Event(MouseInputData(shMouseInput::E::kRight,
                              shButtonState::E::kPressed,
                              ModifierState(modifiers & MK_CONTROL,
                                            modifiers & MK_ALT,
                                            modifiers & MK_SHIFT,
                                            modifiers & 0)));
    break;
  }
  case WM_RBUTTONUP:
  {
    short modifiers = LOWORD(wParam);
    ev = Event(MouseInputData(shMouseInput::E::kRight,
                              shButtonState::E::kReleased,
                              ModifierState(modifiers & MK_CONTROL,
                                            modifiers & MK_ALT,
                                            modifiers & MK_SHIFT,
                                            modifiers & 0)));
    break;
  }
  case WM_XBUTTONDOWN:
  {
    short modifiers = LOWORD(wParam);
    short x = HIWORD(wParam);
    ev = Event(MouseInputData(x & XBUTTON1 ? shMouseInput::E::kButton4 : shMouseInput::E::kButton5,
                              shButtonState::E::kPressed,
                              ModifierState(modifiers & MK_CONTROL,
                                            modifiers & MK_ALT,
                                            modifiers & MK_SHIFT,
                                            modifiers & 0)));
    break;
  }
  case WM_XBUTTONUP:
  {
    short modifiers = LOWORD(wParam);
    short x = HIWORD(wParam);
    ev = Event(MouseInputData(x & XBUTTON1 ? shMouseInput::E::kButton4 : shMouseInput::E::kButton5,
                              shButtonState::E::kReleased,
                              ModifierState(modifiers & MK_CONTROL,
                                            modifiers & MK_ALT,
                                            modifiers & MK_SHIFT,
                                            modifiers & 0)));
    break;
  }
  case WM_INPUT:
  {
    UINT dwSize;
    GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam),
                    RID_INPUT,
                    NULL,
                    &dwSize,
                    sizeof(RAWINPUTHEADER));

    LPBYTE lpb = new BYTE(static_cast<BYTE>(dwSize));
    if (lpb == NULL) {
      return result;
    }

    if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam),
                        RID_INPUT,
                        lpb,
                        &dwSize,
                        sizeof(RAWINPUTHEADER) != dwSize)) {
      OutputDebugString(TEXT("GetRawInputData does not return correct size!\n"));
    }

    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

    if (raw->header.dwType == RIM_TYPEKEYBOARD) {
      raw->data.keyboard.MakeCode,
      raw->data.keyboard.Flags,
      raw->data.keyboard.Reserved,
      raw->data.keyboard.ExtraInformation,
      raw->data.keyboard.Message,
      raw->data.keyboard.VKey;
    }
    else if (raw->header.dwType == RIM_TYPEMOUSE) {
      raw->data.mouse.usFlags,
      raw->data.mouse.ulButtons,
      raw->data.mouse.usButtonFlags,
      raw->data.mouse.usButtonData,
      raw->data.mouse.ulRawButtons,
      raw->data.mouse.lLastX,
      raw->data.mouse.lLastY,
      raw->data.mouse.ulExtraInformation;

      ev = Event(MouseRawData(static_cast<int32>(raw->data.mouse.lLastX),
                              static_cast<int32>(raw->data.mouse.lLastY)));
    }

    delete[] lpb;
    break;
  }
  case WM_MOUSEMOVE:
  {
    int32 x = static_cast<short>(LOWORD(lParam));
    int32 y = static_cast<short>(HIWORD(lParam));
    RECT area;
    
    GetClientRect(hwnd, &area);

    ev = Event(MouseMoveData(static_cast<uint32>(area.left <= x && x <= area.right ? x - area.left : 0xFFFFFFFF),
                             static_cast<uint32>(area.top <= y && y <= area.bottom ? x - area.top : 0xFFFFFFFF),
                             static_cast<uint32>(x),
                             static_cast<uint32>(y),
                             static_cast<uint32>(x - eventQ->getPreviousMouseX()),
                             static_cast<uint32>(y - eventQ->getPreviousMouseY())));

    eventQ->setPreviousMouseX(x);
    eventQ->setPreviousMouseY(y);

    break;
  }
  case WM_KEYDOWN:
  case WM_KEYUP:
  case WM_CHAR:
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
  {
    shKey::E key;

    switch (wParam)
    {
    case VK_ESCAPE:
      key = shKey::E::kEscape;
      break;
    case 0x30:
      key = shKey::E::kNum0;
      break;
    case 0x31:
      key = shKey::E::kNum1;
      break;
    case 0x32:
      key = shKey::E::kNum2;
      break;
    case 0x33:
      key = shKey::E::kNum3;
      break;
    case 0x34:
      key = shKey::E::kNum4;
      break;
    case 0x35:
      key = shKey::E::kNum5;
      break;
    case 0x36:
      key = shKey::E::kNum6;
      break;
    case 0x37:
      key = shKey::E::kNum7;
      break;
    case 0x38:
      key = shKey::E::kNum8;
      break;
    case 0x39:
      key = shKey::E::kNum9;
      break;
    case 0x41:
      key = shKey::E::kA;
      break;
    case 0x42:
      key = shKey::E::kB;
      break;
    case 0x43:
      key = shKey::E::kC;
      break;
    case 0x44:
      key = shKey::E::kD;
      break;
    case 0x45:
      key = shKey::E::kE;
      break;
    case 0x46:
      key = shKey::E::kF;
      break;
    case 0x47:
      key = shKey::E::kG;
      break;
    case 0x48:
      key = shKey::E::kH;
      break;
    case 0x49:
      key = shKey::E::kI;
      break;
    case 0x4A:
      key = shKey::E::kJ;
      break;
    case 0x4B:
      key = shKey::E::kK;
      break;
    case 0x4C:
      key = shKey::E::kL;
      break;
    case 0x4D:
      key = shKey::E::kM;
      break;
    case 0x4E:
      key = shKey::E::kN;
      break;
    case 0x4F:
      key = shKey::E::kO;
      break;
    case 0x50:
      key = shKey::E::kP;
      break;
    case 0x51:
      key = shKey::E::kQ;
      break;
    case 0x52:
      key = shKey::E::kR;
      break;
    case 0x53:
      key = shKey::E::kS;
      break;
    case 0x54:
      key = shKey::E::kT;
      break;
    case 0x55:
      key = shKey::E::kU;
      break;
    case 0x56:
      key = shKey::E::kV;
      break;
    case 0x57:
      key = shKey::E::kW;
      break;
    case 0x58:
      key = shKey::E::kX;
      break;
    case 0x59:
      key = shKey::E::kY;
      break;
    case 0x5A:
      key = shKey::E::kZ;
      break;
    case VK_SUBTRACT:
    case VK_OEM_MINUS:
      key = shKey::E::kMinus;
      break;
    case VK_ADD:
    case VK_OEM_PLUS:
      key = shKey::E::kAdd;
      break;
    case VK_MULTIPLY:
      key = shKey::E::kMultiply;
      break;
    case VK_DIVIDE:
      key = shKey::E::kDivide;
      break;
    case VK_BACK:
      key = shKey::E::kBack;
      break;
    case VK_RETURN:
      key = shKey::E::kEnter;
      break;
    case VK_DELETE:
      key = shKey::E::kDel;
      break;
    case VK_TAB:
      key = shKey::E::kTab;
      break;
    case VK_NUMPAD0:
      key = shKey::E::kNumpad0;
      break;
    case VK_NUMPAD1:
      key = shKey::E::kNumpad1;
      break;
    case VK_NUMPAD2:
      key = shKey::E::kNumpad2;
      break;
    case VK_NUMPAD3:
      key = shKey::E::kNumpad3;
      break;
    case VK_NUMPAD4:
      key = shKey::E::kNumpad4;
      break;
    case VK_NUMPAD5:
      key = shKey::E::kNumpad5;
      break;
    case VK_NUMPAD6:
      key = shKey::E::kNumpad6;
      break;
    case VK_NUMPAD7:
      key = shKey::E::kNumpad7;
      break;
    case VK_NUMPAD8:
      key = shKey::E::kNumpad8;
      break;
    case VK_NUMPAD9:
      key = shKey::E::kNumpad9;
      break;
    case VK_UP:
      key = shKey::E::kUp;
      break;
    case VK_LEFT:
      key = shKey::E::kLeft;
      break;
    case VK_DOWN:
      key = shKey::E::kDown;
      break;
    case VK_RIGHT:
      key = shKey::E::kRight;
      break;
    case VK_SPACE:
      key = shKey::E::kSpace;
      break;
    case VK_HOME:
      key = shKey::E::kHome;
      break;
    case VK_F1:
      key = shKey::E::kF1;
      break;
    case VK_F2:
      key = shKey::E::kF2;
      break;
    case VK_F3:
      key = shKey::E::kF3;
      break;
    case VK_F4:
      key = shKey::E::kF4;
      break;
    case VK_F5:
      key = shKey::E::kF5;
      break;
    case VK_F6:
      key = shKey::E::kF6;
      break;
    case VK_F7:
      key = shKey::E::kF7;
      break;
    case VK_F8:
      key = shKey::E::kF8;
      break;
    case VK_F9:
      key = shKey::E::kF9;
      break;
    case VK_F10:
      key = shKey::E::kF10;
      break;
    case VK_F11:
      key = shKey::E::kF11;
      break;
    case VK_F12:
      key = shKey::E::kF12;
      break;
    case VK_SHIFT:
    case VK_LSHIFT:
    case VK_RSHIFT:
      key = shKey::E::kLShift;
      break;
    case VK_CONTROL:
    case VK_LCONTROL:
    case VK_RCONTROL:
      key = shKey::E::kLControl;
      break;
    case VK_MENU:
    case VK_LMENU:
    case VK_RMENU:
      key = shKey::E::kLAlt;
      break;
    case VK_LWIN:
    case VK_RWIN:
      key = shKey::E::kLWin;
      break;
    case VK_OEM_PERIOD:
      key = shKey::E::kPeriod;
      break;
    case VK_OEM_COMMA:
      key = shKey::E::kComma;
      break;
    case VK_OEM_1:
      key = shKey::E::kSemicolon;
      break;
    case VK_OEM_2:
      key = shKey::E::kBackslash;
      break;
    case VK_OEM_3:
      key = shKey::E::kGrave;
      break;
    case VK_OEM_4:
      key = shKey::E::kLBracket;
      break;
    case VK_OEM_6:
      key = shKey::E::kRBracket;
      break;
    case VK_OEM_7:
      key = shKey::E::kApostrophe;
      break;
    default:
      key = shKey::E::kKeysMax;
    }

    if (key == shKey::E::kLControl && GetKeyState(VK_RCONTROL)) {
      key = shKey::E::kRControl;
    }
    if (key == shKey::E::kLAlt && GetKeyState(VK_RMENU)) {
      key = shKey::E::kRAlt;
    }
    if (key == shKey::E::kLShift && GetKeyState(VK_RSHIFT)) {
      key = shKey::E::kRShift;
    }
    if (key == shKey::E::kLWin && GetKeyState(VK_RWIN)) {
      key = shKey::E::kRWin;
    }
    ModifierState ms;
    ms.shift = (GetKeyState(VK_SHIFT) & 0x8000) |
               (GetKeyState(VK_CAPITAL) & 0x0001);
    ms.alt = GetKeyState(VK_MENU) & 0x8000;
    ms.ctrl = GetKeyState(VK_CONTROL) & 0x8000;
    ms.meta = false;

    if (ms.shift) {
      if (key == shKey::E::kSemicolon) {
        key = shKey::E::kColon;
      }
      if (key == shKey::E::kApostrophe) {
        key = shKey::E::kQuotation;
      }
    }

    if (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN) {
      ev = Event(KeyboardData(key, shButtonState::kPressed, ms));
    }
    else if (msg == WM_KEYUP || msg == WM_SYSKEYUP) {
      ev = Event(KeyboardData(key, shButtonState::kReleased, ms));
    }

    break;
  }
  case WM_SIZE:
  {
    uint32 width, height;

    width = static_cast<uint32>(static_cast<UINT64>(lParam) & 0xFFFF);
    height = static_cast<uint32>(static_cast<UINT64>(lParam) >> 16);

    ev = Event(ResizeData(width, height, false));

    break;
  }
  case WM_SIZING:
  {
    uint32 width, height;
    PRECT rectp = reinterpret_cast<PRECT>(lParam);

    tagRECT wind, rect;
    GetWindowRect(hwnd, &wind);
    GetClientRect(hwnd, &rect);
    width = rectp->right - rectp->left;
    height = rectp->bottom - rectp->top;

    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_INTERNALPAINT);

    ev = Event(ResizeData(width, height, true));
    result = WVR_REDRAW;
    break;
  }
  case WM_DPICHANGED:
  {
    WORD curDPI = HIWORD(wParam);
    FLOAT fscale = static_cast<float>(curDPI) / USER_DEFAULT_SCREEN_DPI;
    ev = Event(DpiData(fscale));

    if (!IsZoomed(hwnd)) {
      RECT* const prcNewWindow = reinterpret_cast<RECT*>(lParam);

      if (prcNewWindow) {
        currenScreenRect = *prcNewWindow;
      }
    }

    break;
  }
  default:
    // Do nothing
    break;
  }

  if (ev.type != shEventType::kNone) {
    eventQ->getQueue().emplace(ev);
  }

  if (!(currenScreenRect.right == currenScreenRect.left &&
        currenScreenRect.right == currenScreenRect.top &&
        currenScreenRect.right == currenScreenRect.bottom &&
        currenScreenRect.right == -1)) {
    SetWindowPos(hwnd,
                 NULL,
                 currenScreenRect.left,
                 currenScreenRect.top,
                 currenScreenRect.right - currenScreenRect.left,
                 currenScreenRect.bottom - currenScreenRect.top,
                 SWP_NOZORDER | SWP_NOACTIVATE);
  }

  SetWindowLongPtrW(hwnd,
                    0,
                    reinterpret_cast<LONG_PTR>(eventQ));

  return result;
}
}

#endif
