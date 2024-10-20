/*************************************************************/
/*
*  @file    shScreenEventHandle.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/19
*  @brief   Base screen event handler
*
*  Base screen event handler
*/
/*************************************************************/
#pragma once
#include "shPrerequisitesCore.h"
#include "shEvent.h"

namespace shEngineSDK {
namespace shProcessingMode{
enum SH_CORE_EXPORT E
{
  kPoll = 0,
  kDispatch,
  kProcessingModeMax
};
}

/**
*  @brief Class for handle events on screen.
*/
class SH_CORE_EXPORT ScreenEventHandle
{
 public:
  /**
  *  @brief Default constructor.
  */
  ScreenEventHandle() = default;

  /**
  *  @brief Default destructor.
  */
  ~ScreenEventHandle() = default;

  /**
  *  @brief Update events.
  */
  void
  update();

  /**
  *  @brief Get the event queue.
  * 
  *  @return Queue<Event>
  */
  Queue<Event>
  getQueue();

  /**
  *  @brief Checks if queue is empty
  * 
  *  @return bool True if is empty, false is otherwise.
  */
  bool
  empty() const;

  /**
  *  @brief Returns the front element on queue.
  * 
  *  @return Event
  */
  Event
  front();

  /**
  *  @brief Pops the last element on queue.
  */
  void
  pop();

  /**
  *  @brief Emplace element on queue.
  * 
  *  @param Event& ev
  */
  void
  emplace(const Event& ev);

  /**
  *  @brief Set the processing mode
  * 
  *  @param shProcessingMode::E mode
  */
  void
  setProcessingMode(const shProcessingMode::E& mode);

  /**
  *  @brief Return the size
  * 
  *  @return SIZE_T
  */
  SIZE_T
  getSize() const;

  /**
  *  @brief Return the previous mouse position in X axis.
  *
  *  @return uint32
  */
  uint32
  getPreviousMouseX() const;

  /**
  *  @brief Return the previous mouse position in Y axis.
  *
  *  @return uint32
  */
  uint32
  getPreviousMouseY() const;

  /**
  *  @brief Sets the previous mouse position in X axis.
  *
  *  @param uint32 pos
  */
  void
  setPreviousMouseX(uint32 pos);

  /**
  *  @brief Sets the previous mouse position in Y axis.
  *
  *  @param uint32 pos
  */
  void
  setPreviousMouseY(uint32 pos);

  friend class Screen;

 protected:
  /**
  *  @brief Processing mode
  */
  shProcessingMode::E m_processingMode;

  /**
  *  @brief Is initialized?
  */
  bool m_initialized;

  /**
  *  @brief Previous mouse position in X axis.
  */
  uint32 m_prevMouseX;

  /**
  *  @brief Previous mouse position in Y axis.
  */
  uint32 m_prevMouseY;

  /**
  *  @brief Event queue.
  */
  Queue<Event> m_queue;

  /**
  *  @brief Type to transform win32 key to input map.
  */
  typedef shKey::E Win32VKeyToDigitalInputMap[1 << (8 * sizeof(unsigned char))];
  
  /**
  *  @brief Transform win32 key to input map.
  */
  Win32VKeyToDigitalInputMap m_vKeyToDigitalInputMap = {
    /*NONE 0x00*/ shKey::E::kKeysMax,
    /*VK_LBUTTON 0x01*/ shKey::E::kKeysMax,
    /*VK_RBUTTON 0x02*/ shKey::E::kKeysMax,
    /*VK_CANCEL 0x03*/ shKey::E::kKeysMax
  };
};
}
