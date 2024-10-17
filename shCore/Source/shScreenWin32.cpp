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
LONG_PTR CALLBACK
windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool
Screen::init(ScreenDesc& desc, SPtr<ScreenEventHandle> eventHandler)
{
  HINSTANCE hinstance = GetModuleHandle(nullptr);

  WNDCLASS wc = { };
  wc.lpfnWndProc = windowProc;
  wc.hInstance = hinstance;
  wc.lpszClassName = desc.name.c_str();
  wc.cbWndExtra = sizeof(ScreenEventHandle*);

  RegisterClass(&wc);

  HWND hwnd = CreateWindowEx(WS_EX_APPWINDOW,
                                  desc.name.c_str(),
                                  desc.title.c_str(),
                                  WS_OVERLAPPEDWINDOW,
                                  desc.positionX,
                                  desc.positionY,
                                  desc.width,
                                  desc.height,
                                  NULL,
                                  NULL,
                                  hinstance,
                                  NULL);

  m_screenHandle = reinterpret_cast<HWND__*>(hwnd);

  if (m_screenHandle == NULL) {
    SH_ASSERT("Screen.init()");
    return false;
  }

  ShowWindow(reinterpret_cast<HWND>(m_screenHandle), SW_SHOW);

  SetForegroundWindow(reinterpret_cast<HWND>(m_screenHandle));

  SetFocus(reinterpret_cast<HWND>(m_screenHandle));

  SetWindowLongPtrW(reinterpret_cast<HWND>(m_screenHandle),
                    0,
                    reinterpret_cast<LONG_PTR>(eventHandler.get()));

  return true;
}

void
Screen::close()
{
  DestroyWindow(reinterpret_cast<HWND>(m_screenHandle));
  PostQuitMessage(0);
}
}

#endif
