/*************************************************************/
/*
*  @file    shModule.h
*  @author  Samuel Prince (samuel.price.quezada@gmail.com)
*  @date    2024/10/03
*  @brief   Represents one engine module. Essentially it is a specialized type
*           of singleton. Module must be manually started up and shutdown
*           before and after use.
*
*  Represents one engine module. Essentially it is a specialized type
*  of singleton. Module must be manually started up and shutdown
*  before and after use.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesUtilities.h"

namespace shEngineSDK {
/**
*  @brief Represents one engine module. Essentially it is a specialized type
*         of singleton. Module must be manually started up and shutdown
*         before and after use.
*/
template <class T>
class Module
{
 public:

  /**
  *  @brief Returns a reference to the module instance. Module has to have been
  *         started up first otherwise an exception will be thrown.
  */
  static T&
  instance()
  {
    if (!isStartedUp()) {
      SH_ASSERT("Trying to access a module but it hasn't been started");
    }
  
    if (!isDestroyed()) {
      SH_ASSERT("Trying to access a destroyed module");
    }
  
    return *_instance();
  }
  
  /**
  *  @brief Returns a pointer to the module instance. Module has to have been
  *         started up first otherwise an exception will be thrown.
  */
  static T*
  instancePtr()
  {
    if (!isStartedUp()) {
      SH_ASSERT("Trying to access a module but it hasn't been started");
    }
  
    if (!isDestroyed()) {
      SH_ASSERT("Trying to access a destroyed module");
    }
  
    return _instance();
  }
  
  /**
  *  @brief Constructs and starts the module using the specified parameters.
  */
  template <class... Args>
  static void
  startUp(Args&& ...args)
  {
    if (isStartedUp()) {
      SH_ASSERT("Trying to start an already started module.");
    }

    _instance() = new T(std::forward<Args>(args)...);
    isStartedUp() = true;

    static_cast<Module*>(_instance())->onStartUp();
  }

  /**
  *  @brief Constructs and starts a specialized type of the module.
  *         Provide type must derive from type the Module is initialized with.
  */
  template <class SubType, class... Args>
  static void
  startUp(Args&& ...args)
  {
    SH_ASSERT("Provided type isn't derived from type the Module is initialized with.");

    if (isStartedUp()) {
      SH_ASSERT("Trying to start an already started module");
    }

    _instance() = new SubType(std::forward<Args>(args)...);
    isStartedUp() = true;

    static_cast<Module*>(_instance())->onStartUp();
  }

  /**
  *  @brief Shuts down this module and frees any resources it is using.
  */
  static void
  shutDown()
  {
    if (isDestroyed()) {
      SH_ASSERT("Trying to shutdown an already shutdown module");
    }

    if (!isStartedUp()) {
      SH_ASSERT("Trying to shutdown a module which was never started");
    }

    static_cast<Module*>(_instance())->onShutDown();

    delete(_instance());
    isDestroyed() = true;
  }

  /**
  *  @brief Query if the module has been started.
  */
  static bool
  isStarted()
  {
    return isStartedUp() && !isDestroyed();
  }

 protected:

  virtual
  ~Module() = default;

  Module(Module&&) = delete;

  Module(const Module&) = delete;

  Module&
  operator=(Module&&) = delete;

  Module&
  operator=(const Module&) = delete;

  /**
  * @brief Override if you want your module to be notified once it has been
  *        constructed and started.
  */
  virtual void
  onStartUp() {}

  /**
  *  @brief Override if you want your module to be notified just before it is deleted.
  */
  virtual void
  onShutDown() {}

  /**
  *  @brief Returns a singleton instance of this module.
  *         Throws an exception if module is not yet initialized.
  */
  static T*&
  _instance()
  {
    static T* inst = nullptr;
    return inst;
  }

  /**
  *  @brief Checks if the module is shutdown.
  *  @note If module was never started, this will reutn false.
  */
  static bool&
  isDestroyed()
  {
    static bool inst = false;
    return inst;
  }

  /**
  *  @brief Checks if the module is started.
  */
  static bool&
  isStartedUp()
  {
    static bool inst = false;
    return inst;
  }
};
}
