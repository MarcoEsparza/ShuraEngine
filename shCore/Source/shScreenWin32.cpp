/*************************************************************/
/*
*  @file    shScreenWin32.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/17
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

  WNDCLASSEX wc = { };
  wc.lpfnWndProc = windowProc;
  wc.hInstance = hinstance;
  wc.lpszClassName = desc.name.c_str();
  wc.cbWndExtra = sizeof(ScreenEventHandle*);
  wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
  wc.style = CS_DBLCLKS;
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.lpszMenuName = NULL;
  wc.cbClsExtra = 0;

  if (desc.iconPath != "") {
    HICON hIcon = reinterpret_cast<HICON>(::LoadImage(hinstance,
                                          desc.iconPath.c_str(),
                                          IMAGE_ICON,
                                          32,
                                          32,
                                          LR_LOADFROMFILE));
    if (!hIcon) {
      MessageBox(nullptr, "Couldn't load image", "Error", MB_ICONERROR);
    }

    wc.hIcon = hIcon;
    wc.hIconSm = hIcon;
  }
  else {
    wc.hIcon = NULL;
    wc.hIconSm = NULL;
  }

  RegisterClassEx(&wc);

  m_screenHandle = reinterpret_cast<HWND__*>(CreateWindowEx(WS_EX_APPWINDOW,
                                                            desc.name.c_str(),
                                                            desc.title.c_str(),
                                                            WS_OVERLAPPEDWINDOW,
                                                            desc.positionX,
                                                            desc.positionY,
                                                            desc.width,
                                                            desc.height,
                                                            nullptr,
                                                            nullptr,
                                                            hinstance,
                                                            nullptr));

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
