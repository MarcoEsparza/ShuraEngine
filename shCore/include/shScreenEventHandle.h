/*****************************************************************************/
/*
*  @file    shScreenEventHandle.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/07/17
*  @brief   Base screen event handler
*
*  Base screen event handler
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
*  Includes
*/
/*****************************************************************************/
#include "shPrerequisitesCore.h"
#include "shEvent.h"

namespace shEngineSDK {
namespace shPROCESSING_MODE{
enum E
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

  /***************************************************************************/
  /*
  *  Functions
  */
  /***************************************************************************/
 public:
  /**
  *  @brief Update events.
  */
  void
  update();

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
  *  @return const Event
  */
  const Event&
  front() const;

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
  setProcessingMode(const shPROCESSING_MODE::E& mode);

  /**
  *  @brief Return the size
  * 
  *  @return SIZE_T
  */
  SIZE_T
  getSize() const;

  friend class Screen;

  /***************************************************************************/
  /*
  *  Variables
  */
  /***************************************************************************/
 protected:
  /**
  *  @brief Processing mode
  */
  shPROCESSING_MODE::E m_processingMode;

  /**
  *  @brief Is initialized?
  */
  bool m_initialized;

  /**
  *  @brief Event queue.
  */
  Queue<Event> m_queue;
};
}
