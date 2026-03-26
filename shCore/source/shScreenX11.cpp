/*****************************************************************************/
/*
*  @file    shScreenX11.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/19
*  @brief   Base window for Linux.
*
*  Base window for Linux.
*/
/*****************************************************************************/

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shScreen.h"

#if SH_PLATFORM == SH_PLATFORM_LINUX

#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct LinuxScreenHandle
{
  Display* display;
  Window window;
  Atom wmDelete;
};

namespace shEngineSDK{
bool
Screen::init(const ScreenDesc& desc, const SPtr<ScreenEventHandle>& eventHandler)
{
  m_eventQueue = eventHandler;
  m_width = desc.width;
  m_height = desc.height;
  m_fullscreen = desc.fullscreen;

  XInitThreads();
  m_screenHandle = new LinuxScreenHandle();
  m_screenHandle->display = XOpenDisplay(nullptr);
  int32 screen = DefaultScreen(m_screenHandle->display);
  Visual* visual = DefaultVisual(m_screenHandle->display, screen);
  int32 depth = DefaultDepth(m_screenHandle->display, screen);

  Colormap colormap = XCreateColormap(m_screenHandle->display,
                                      RootWindow(m_screenHandle->display, screen),
                                      visual, AllocNone);

  XSetWindowAttributes windowAttributes = {};
  windowAttributes.colormap = colormap;
  windowAttributes.background_pixel = 0xFFFFFFFF;
  windowAttributes.border_pixel = 0;
  windowAttributes.event_mask = KeyPressMask |
                                KeyReleaseMask |
                                StructureNotifyMask |
                                ExposureMask;

  m_screenHandle->window = XCreateWindow(m_screenHandle->display,
                                         RootWindow(m_screenHandle->display, screen),
                                         0, 0, m_width, m_height, 0,
                                         depth, InputOutput, visual,
                                         CWColormap | CWBorderPixel | CWEventMask,
                                         &windowAttributes);

  XSizeHints sizeHints = {};
  sizeHints.flags = PMinSize;
  sizeHints.min_width = 200;
  sizeHints.min_height = 200;
  XSetWMNormalHints(m_screenHandle->display, m_screenHandle->window, &sizeHints);
  m_screenHandle->wmDelete = XInternAtom(m_screenHandle->display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(m_screenHandle->display, m_screenHandle->window, &m_screenHandle->wmDelete, 1);                                        
  XSelectInput(m_screenHandle->display, m_screenHandle->window,
               KeyPressMask |
               ExposureMask |
               ButtonPressMask |
               ButtonReleaseMask |
               PointerMotionMask);
  XMapWindow(m_screenHandle->display, m_screenHandle->window);
  XFlush(m_screenHandle->display);

  return true;
}

void
Screen::close()
{
  if (m_screenHandle) {
    XDestroyWindow(m_screenHandle->display, m_screenHandle->window);
    XCloseDisplay(m_screenHandle->display);
    delete m_screenHandle;
    m_screenHandle = nullptr;
  }
}

Vector2i
Screen::getClientSize() const
{
  return Vector2i(m_width, m_height);
}
}

#endif
