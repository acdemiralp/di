#ifndef NANO_ENGINE_SYSTEMS_INPUT_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_HPP_

#include <iostream>

#include <boost/signals2.hpp>
#include <SDL2/SDL.h>

#include <nano_engine/engine.hpp>
#include <nano_engine/system.hpp>

namespace ne
{
class input : public system
{
public:
  boost::signals2::signal<void()>                   on_quit          ;
  boost::signals2::signal<void(char)>               on_key_pressed   ;
  boost::signals2::signal<void(char)>               on_key_released  ;
  boost::signals2::signal<void(unsigned, unsigned)> on_mouse_moved   ;
  boost::signals2::signal<void(unsigned)>           on_mouse_pressed ;
  boost::signals2::signal<void(unsigned)>           on_mouse_released;

  bool get_key_state(unsigned char key ) const
  {
    return key_states_[key] != 0;
  }
  void lock_mouse   (bool          lock)
  {
    if (SDL_SetRelativeMouseMode(SDL_bool(lock)) != 0)
      std::cout << "Unable to lock mouse. SDL Error: " << SDL_GetError() << std::endl;
  }

protected:
  void initialize() override
  {
    if (SDL_Init(SDL_INIT_EVENTS) < 0)
    {
      std::cout << "Failed to initialize input system. SDL Error: " << SDL_GetError() << std::endl;
      return;
    }
    on_quit.connect(std::bind(&engine::stop, engine_));
  }
  void update    () override
  {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
      switch (event.type)
      {
        case SDL_QUIT           : on_quit();                                               break;
        case SDL_KEYDOWN        : on_key_pressed   (event.key.keysym.sym);                 break;
        case SDL_KEYUP          : on_key_released  (event.key.keysym.sym);                 break;
        case SDL_MOUSEBUTTONDOWN: on_mouse_pressed (event.button.button);                  break;
        case SDL_MOUSEBUTTONUP  : on_mouse_released(event.button.button);                  break;
        case SDL_MOUSEMOTION    : on_mouse_moved   (event.motion.xrel, event.motion.yrel); break;
        default: break;
      }
    }
    key_states_ = const_cast<unsigned char*>(SDL_GetKeyboardState(nullptr));
  }
  void terminate () override
  {
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
  }

  unsigned char* key_states_;
};
}

#endif
