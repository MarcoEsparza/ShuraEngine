/*****************************************************************************/
/*
*  @file    shScreenEventHandle.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/17
*  @brief   Base screen event handler for windows.
*
*  Base screen event handler for windows.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shScreenEventHandle.h"
#include <shVector2i.h>

#if SH_PLATFORM == SH_PLATFORM_WIN32

#include <Windows.h>

namespace shEngineSDK {
void
ScreenEventHandle::update()
{
  MSG msg = { };
  if (m_processingMode == shPROCESSING_MODE::E::kPoll) {
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        return;
      }
      
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  else {
    GetMessage(&msg, nullptr, 0, 0);
  }
}

bool
ScreenEventHandle::empty() const
{
  return m_queue.empty();
}

const Event&
ScreenEventHandle::front() const
{
  return m_queue.front();
}

void
ScreenEventHandle::pop()
{
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

SIZE_T
ScreenEventHandle::getSize() const
{
  return m_queue.size();
}

LRESULT CALLBACK
windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  LRESULT result = DefWindowProc(hwnd, msg, wParam, lParam);
  RECT currenScreenRect = { -1,-1,-1,-1 };

  Event currentEvent = Event(EVENT_TYPE::E::kNone);

  ScreenEventHandle* eventQ =
  reinterpret_cast<ScreenEventHandle*>(GetWindowLongPtrA(hwnd, 0));
  Vector2i* prevMousePos = reinterpret_cast<Vector2i*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));

  switch (msg) {
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

    currentEvent = Event(EVENT_TYPE::E::kPaint);
    break;
  }
  case WM_ERASEBKGND:
  {
    break;
  }
  case WM_CLOSE:
  case WM_DESTROY:
  {
    currentEvent = Event(EVENT_TYPE::E::kClose);
    break;
  }
  case WM_SETFOCUS:
  {
    currentEvent = Event(true);
    break;
  }
  case WM_KILLFOCUS:
  {
    currentEvent = Event(false);
    break;
  }
  case WM_MOUSEWHEEL:
  {
    short modifiers = LOWORD(wParam);
    currentEvent = Event(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA,
                         ModifierState(modifiers & MK_CONTROL,
                                       modifiers & MK_ALT,
                                       modifiers & MK_SHIFT,
                                       modifiers & 0));
    break;
  }
  case WM_MOUSEHWHEEL:
  {
    short modifiers = LOWORD(wParam);
    currentEvent = Event(-GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA,
                         ModifierState(modifiers & MK_CONTROL,
                                       modifiers & MK_ALT,
                                       modifiers & MK_SHIFT,
                                       modifiers & 0),
                         true);
    break;
  }
  case WM_LBUTTONDOWN:
  {
    short modifiers = LOWORD(wParam);
    currentEvent = Event(MOUSE_INPUT::E::kLeft,
                         BUTTON_STATE::E::kPressed,
                         ModifierState(modifiers & MK_CONTROL,
                                       modifiers & MK_ALT,
                                       modifiers & MK_SHIFT,
                                       modifiers & 0));
    break;
  }
  case WM_LBUTTONUP:
  {
    short modifiers = LOWORD(wParam);
    currentEvent = Event(MOUSE_INPUT::E::kLeft,
                         BUTTON_STATE::E::kReleased,
                         ModifierState(modifiers & MK_CONTROL,
                                       modifiers & MK_ALT,
                                       modifiers & MK_SHIFT,
                                       modifiers & 0));
    break;
  }
  case WM_MBUTTONDOWN:
  {
    short modifiers = LOWORD(wParam);
    currentEvent = Event(MOUSE_INPUT::E::kMiddle,
                         BUTTON_STATE::E::kPressed,
                         ModifierState(modifiers & MK_CONTROL,
                                       modifiers & MK_ALT,
                                       modifiers & MK_SHIFT,
                                       modifiers & 0));
    break;
  }
  case WM_MBUTTONUP:
  {
    short modifiers = LOWORD(wParam);
    currentEvent = Event(MOUSE_INPUT::E::kMiddle,
                         BUTTON_STATE::E::kReleased,
                         ModifierState(modifiers & MK_CONTROL,
                                       modifiers & MK_ALT,
                                       modifiers & MK_SHIFT,
                                       modifiers & 0));
    break;
  }
  case WM_RBUTTONDOWN:
  {
    short modifiers = LOWORD(wParam);
    currentEvent = Event(MOUSE_INPUT::E::kRight,
                         BUTTON_STATE::E::kPressed,
                         ModifierState(modifiers & MK_CONTROL,
                                       modifiers & MK_ALT,
                                       modifiers & MK_SHIFT,
                                       modifiers & 0));
    break;
  }
  case WM_RBUTTONUP:
  {
    short modifiers = LOWORD(wParam);
    currentEvent = Event(MOUSE_INPUT::E::kRight,
                         BUTTON_STATE::E::kReleased,
                         ModifierState(modifiers & MK_CONTROL,
                                       modifiers & MK_ALT,
                                       modifiers & MK_SHIFT,
                                       modifiers & 0));
    break;
  }
  case WM_XBUTTONDOWN:
  {
    short modifiers = LOWORD(wParam);
    short x = HIWORD(wParam);
    currentEvent = Event(x & XBUTTON1 ? MOUSE_INPUT::E::kButton4 :
                         MOUSE_INPUT::E::kButton5,
                         BUTTON_STATE::E::kPressed,
                         ModifierState(modifiers & MK_CONTROL,
                                       modifiers & MK_ALT,
                                       modifiers & MK_SHIFT,
                                       modifiers & 0));
    break;
  }
  case WM_XBUTTONUP:
  {
    short modifiers = LOWORD(wParam);
    short x = HIWORD(wParam);
    currentEvent = Event(x & XBUTTON1 ? MOUSE_INPUT::E::kButton4 :
                         MOUSE_INPUT::E::kButton5,
                         BUTTON_STATE::E::kReleased,
                         ModifierState(modifiers & MK_CONTROL,
                                       modifiers & MK_ALT,
                                       modifiers & MK_SHIFT,
                                       modifiers & 0));
    break;
  }
  case WM_INPUT:
  {
    UINT dwSize;
    GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam),
                    RID_INPUT,
                    nullptr,
                    &dwSize,
                    sizeof(RAWINPUTHEADER));

    Byte* lpByte = new Byte(static_cast<Byte>(dwSize));

    if (lpByte == nullptr) {
      return result;
    }

    if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam),
                        RID_INPUT,
                        lpByte,
                        &dwSize,
                        sizeof(RAWINPUTHEADER) != dwSize)) {
      OutputDebugString(TEXT("GetRawInputData does not return correct size!\n"));
    }

    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpByte);

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

      currentEvent = Event(static_cast<int32>(raw->data.mouse.lLastX),
                           static_cast<int32>(raw->data.mouse.lLastY));
    }

    delete lpByte;
    break;
  }
  case WM_MOUSEMOVE:
  {
    int32 x = static_cast<int32>(LOWORD(lParam));
    int32 y = static_cast<int32>(HIWORD(lParam));
    RECT area;
    
    GetClientRect(hwnd, &area);

    if (prevMousePos != nullptr) {
      currentEvent = Event(static_cast<uint32>(area.left <= x && x <=
                           area.right ? x - area.left : 0xFFFFFFFF),
                           static_cast<uint32>(area.top <= y && y <=
                           area.bottom ? y - area.top : 0xFFFFFFFF),
                           static_cast<uint32>(x),
                           static_cast<uint32>(y),
                           static_cast<uint32>(x - prevMousePos->x),
                           static_cast<uint32>(y - prevMousePos->y));

      prevMousePos->x = x;
      prevMousePos->y = y;
    }

    break;
  }
  case WM_KEYDOWN:
  case WM_KEYUP:
  case WM_CHAR:
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
  {
    KEY::E key;

    switch (wParam)
    {
    case VK_ESCAPE:
      key = KEY::E::kEscape;
      break;
    case 0x30:
      key = KEY::E::kNum0;
      break;
    case 0x31:
      key = KEY::E::kNum1;
      break;
    case 0x32:
      key = KEY::E::kNum2;
      break;
    case 0x33:
      key = KEY::E::kNum3;
      break;
    case 0x34:
      key = KEY::E::kNum4;
      break;
    case 0x35:
      key = KEY::E::kNum5;
      break;
    case 0x36:
      key = KEY::E::kNum6;
      break;
    case 0x37:
      key = KEY::E::kNum7;
      break;
    case 0x38:
      key = KEY::E::kNum8;
      break;
    case 0x39:
      key = KEY::E::kNum9;
      break;
    case 0x41:
      key = KEY::E::kA;
      break;
    case 0x42:
      key = KEY::E::kB;
      break;
    case 0x43:
      key = KEY::E::kC;
      break;
    case 0x44:
      key = KEY::E::kD;
      break;
    case 0x45:
      key = KEY::E::kE;
      break;
    case 0x46:
      key = KEY::E::kF;
      break;
    case 0x47:
      key = KEY::E::kG;
      break;
    case 0x48:
      key = KEY::E::kH;
      break;
    case 0x49:
      key = KEY::E::kI;
      break;
    case 0x4A:
      key = KEY::E::kJ;
      break;
    case 0x4B:
      key = KEY::E::kK;
      break;
    case 0x4C:
      key = KEY::E::kL;
      break;
    case 0x4D:
      key = KEY::E::kM;
      break;
    case 0x4E:
      key = KEY::E::kN;
      break;
    case 0x4F:
      key = KEY::E::kO;
      break;
    case 0x50:
      key = KEY::E::kP;
      break;
    case 0x51:
      key = KEY::E::kQ;
      break;
    case 0x52:
      key = KEY::E::kR;
      break;
    case 0x53:
      key = KEY::E::kS;
      break;
    case 0x54:
      key = KEY::E::kT;
      break;
    case 0x55:
      key = KEY::E::kU;
      break;
    case 0x56:
      key = KEY::E::kV;
      break;
    case 0x57:
      key = KEY::E::kW;
      break;
    case 0x58:
      key = KEY::E::kX;
      break;
    case 0x59:
      key = KEY::E::kY;
      break;
    case 0x5A:
      key = KEY::E::kZ;
      break;
    case VK_SUBTRACT:
    case VK_OEM_MINUS:
      key = KEY::E::kMinus;
      break;
    case VK_ADD:
    case VK_OEM_PLUS:
      key = KEY::E::kAdd;
      break;
    case VK_MULTIPLY:
      key = KEY::E::kMultiply;
      break;
    case VK_DIVIDE:
      key = KEY::E::kDivide;
      break;
    case VK_BACK:
      key = KEY::E::kBack;
      break;
    case VK_RETURN:
      key = KEY::E::kEnter;
      break;
    case VK_DELETE:
      key = KEY::E::kDel;
      break;
    case VK_TAB:
      key = KEY::E::kTab;
      break;
    case VK_NUMPAD0:
      key = KEY::E::kNumpad0;
      break;
    case VK_NUMPAD1:
      key = KEY::E::kNumpad1;
      break;
    case VK_NUMPAD2:
      key = KEY::E::kNumpad2;
      break;
    case VK_NUMPAD3:
      key = KEY::E::kNumpad3;
      break;
    case VK_NUMPAD4:
      key = KEY::E::kNumpad4;
      break;
    case VK_NUMPAD5:
      key = KEY::E::kNumpad5;
      break;
    case VK_NUMPAD6:
      key = KEY::E::kNumpad6;
      break;
    case VK_NUMPAD7:
      key = KEY::E::kNumpad7;
      break;
    case VK_NUMPAD8:
      key = KEY::E::kNumpad8;
      break;
    case VK_NUMPAD9:
      key = KEY::E::kNumpad9;
      break;
    case VK_UP:
      key = KEY::E::kUp;
      break;
    case VK_LEFT:
      key = KEY::E::kLeft;
      break;
    case VK_DOWN:
      key = KEY::E::kDown;
      break;
    case VK_RIGHT:
      key = KEY::E::kRight;
      break;
    case VK_SPACE:
      key = KEY::E::kSpace;
      break;
    case VK_HOME:
      key = KEY::E::kHome;
      break;
    case VK_F1:
      key = KEY::E::kF1;
      break;
    case VK_F2:
      key = KEY::E::kF2;
      break;
    case VK_F3:
      key = KEY::E::kF3;
      break;
    case VK_F4:
      key = KEY::E::kF4;
      break;
    case VK_F5:
      key = KEY::E::kF5;
      break;
    case VK_F6:
      key = KEY::E::kF6;
      break;
    case VK_F7:
      key = KEY::E::kF7;
      break;
    case VK_F8:
      key = KEY::E::kF8;
      break;
    case VK_F9:
      key = KEY::E::kF9;
      break;
    case VK_F10:
      key = KEY::E::kF10;
      break;
    case VK_F11:
      key = KEY::E::kF11;
      break;
    case VK_F12:
      key = KEY::E::kF12;
      break;
    case VK_SHIFT:
    case VK_LSHIFT:
    case VK_RSHIFT:
      key = KEY::E::kLShift;
      break;
    case VK_CONTROL:
    case VK_LCONTROL:
    case VK_RCONTROL:
      key = KEY::E::kLControl;
      break;
    case VK_MENU:
    case VK_LMENU:
    case VK_RMENU:
      key = KEY::E::kLAlt;
      break;
    case VK_LWIN:
    case VK_RWIN:
      key = KEY::E::kLWin;
      break;
    case VK_OEM_PERIOD:
      key = KEY::E::kPeriod;
      break;
    case VK_OEM_COMMA:
      key = KEY::E::kComma;
      break;
    case VK_OEM_1:
      key = KEY::E::kSemicolon;
      break;
    case VK_OEM_2:
      key = KEY::E::kBackslash;
      break;
    case VK_OEM_3:
      key = KEY::E::kGrave;
      break;
    case VK_OEM_4:
      key = KEY::E::kLBracket;
      break;
    case VK_OEM_6:
      key = KEY::E::kRBracket;
      break;
    case VK_OEM_7:
      key = KEY::E::kApostrophe;
      break;
    default:
      key = KEY::E::kKeysMax;
    }

    if (key == KEY::E::kLControl && GetKeyState(VK_RCONTROL)) {
      key = KEY::E::kRControl;
    }
    if (key == KEY::E::kLAlt && GetKeyState(VK_RMENU)) {
      key = KEY::E::kRAlt;
    }
    if (key == KEY::E::kLShift && GetKeyState(VK_RSHIFT)) {
      key = KEY::E::kRShift;
    }
    if (key == KEY::E::kLWin && GetKeyState(VK_RWIN)) {
      key = KEY::E::kRWin;
    }
    ModifierState ms;
    ms.shift = (GetKeyState(VK_SHIFT) & 0x8000) |
               (GetKeyState(VK_CAPITAL) & 0x0001);
    ms.alt = GetKeyState(VK_MENU) & 0x8000;
    ms.ctrl = GetKeyState(VK_CONTROL) & 0x8000;
    ms.meta = false;

    if (ms.shift) {
      if (key == KEY::E::kSemicolon) {
        key = KEY::E::kColon;
      }
      if (key == KEY::E::kApostrophe) {
        key = KEY::E::kQuotation;
      }
    }

    if (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN) {
      currentEvent = Event(key, BUTTON_STATE::kPressed, ms);
    }
    else if (msg == WM_KEYUP || msg == WM_SYSKEYUP) {
      currentEvent = Event(key, BUTTON_STATE::kReleased, ms);
    }

    break;
  }
  case WM_SIZE:
  {
    uint32 width, height;

    width = static_cast<uint32>(static_cast<UINT64>(lParam) & 0xFFFF);
    height = static_cast<uint32>(static_cast<UINT64>(lParam) >> 16);

    //currentEvent = Event(width, height, false);
    switch (wParam)
    {
    case SIZE_MINIMIZED:
      break;
    case SIZE_MAXIMIZED:
      currentEvent = Event(width, height, false);
      break;
    case SIZE_RESTORED:
      currentEvent = Event(width, height, false);
      break;
    default:
      break;
    }

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

    RedrawWindow(hwnd,
                 nullptr,
                 nullptr,
                 RDW_INVALIDATE | RDW_NOERASE | RDW_INTERNALPAINT);

    //currentEvent = Event(width, height, true);
    result = WVR_REDRAW;
    break;
  }
  case WM_EXITSIZEMOVE:
  {
    RECT rect;
    GetWindowRect(hwnd, &rect);
    currenScreenRect = rect;
    uint32 width = static_cast<uint32>(rect.right - rect.left);
    uint32 height = static_cast<uint32>(rect.bottom - rect.top);
    currentEvent = Event(width, height, false);
    break;
  }
  case WM_DPICHANGED:
  {
    WORD curDPI = HIWORD(wParam);
    FLOAT fscale = static_cast<float>(curDPI) / USER_DEFAULT_SCREEN_DPI;
    currentEvent = Event(fscale);

    if (!IsZoomed(hwnd)) {
      RECT* const prcNewWindow = reinterpret_cast<RECT*>(lParam);

      if (prcNewWindow) {
        currenScreenRect = *prcNewWindow;
      }
    }

    break;
  }
  case WM_GETMINMAXINFO:
  {
    MINMAXINFO* min_max = reinterpret_cast<MINMAXINFO*>(lParam);

    min_max->ptMinTrackSize.x = 200;
    min_max->ptMinTrackSize.y = 200;

    break;
  }
  default:
    // Do nothing
    break;
  }

  if (currentEvent.type != EVENT_TYPE::E::kNone) {
    if (eventQ != nullptr) {
      eventQ->emplace(currentEvent);
    }
  }

  if (!(currenScreenRect.right == currenScreenRect.left &&
        currenScreenRect.right == currenScreenRect.top &&
        currenScreenRect.right == currenScreenRect.bottom &&
        currenScreenRect.right == -1)) {
    SetWindowPos(hwnd,
                 nullptr,
                 currenScreenRect.left,
                 currenScreenRect.top,
                 currenScreenRect.right - currenScreenRect.left,
                 currenScreenRect.bottom - currenScreenRect.top,
                 SWP_NOZORDER | SWP_NOACTIVATE);
  }

  return result;
}
}

#endif
