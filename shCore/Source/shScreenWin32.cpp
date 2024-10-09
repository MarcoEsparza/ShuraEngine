/*************************************************************/
/*
*  @file    shScreenWin32.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/08
*  @brief   Base screen
*
*  Base screen
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shScreen.h"

#if SH_PLATFORM == SH_PLATFORM_WIN32

#include "Windows.h"

namespace shEngineSDK {
bool
Screen::init(ScreenDesc desc, SPtr<ScreenEventHandle> eventHandler)
{
  HINSTANCE hinstance = GetModuleHandle(nullptr);

  WNDCLASS wc = { };
  wc.lpfnWndProc = win32Proc;
  wc.hInstance = hinstance;
  wc.lpszClassName = desc.name.c_str();
  wc.cbWndExtra = sizeof(ScreenEventHandle*);

  RegisterClass(&wc);
  
  int32 nFrameWidth = ::GetSystemMetrics(SM_CXSIZEFRAME) + 5;
  int32 nFrameHeight = ::GetSystemMetrics(SM_CYSIZEFRAME) + 5;
  int32 nCaptionHeight = ::GetSystemMetrics(SM_CYCAPTION);

  int32 nStyle = WS_OVERLAPPEDWINDOW;

  HWND hwnd = CreateWindowEx(WS_EX_APPWINDOW,
                             desc.name.c_str(),
                             desc.title.c_str(),
                             nStyle,
                             desc.positionX,
                             desc.positionY,
                             desc.width + (2 * nFrameWidth),
                             desc.height + (2 * nFrameHeight) + nCaptionHeight,
                             NULL,
                             NULL,
                             hinstance,
                             NULL);

  if (hwnd == NULL) {
    SH_ASSERT("Screen.init()");
    return false;
  }

  ShowWindow(hwnd, SW_SHOW);

  SetForegroundWindow(hwnd);

  SetFocus(hwnd);

  RECT clientRect;
  GetClientRect(hwnd, &clientRect);
  m_screenHandle = hwnd;
  m_width = clientRect.right - clientRect.left;
  m_height = clientRect.bottom - clientRect.top;

  SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(eventHandler.get()));

  return true;
}

void
Screen::close()
{
  DestroyWindow(reinterpret_cast<HWND>(m_screenHandle));
  PostQuitMessage(0);
}

// TODO: finish this
LONG_PTR CALLBACK
win32Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  MSG message;
  message.hwnd = hwnd;
  message.wParam = wParam;
  message.lParam = lParam;
  message.message = msg;
  message.time = 0;

  //LRESULT result = 
}
}

#endif
