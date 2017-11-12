#ifndef NANO_ENGINE_SYSTEMS_INPUT_INPUT_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_INPUT_HPP_

#include <iostream>

#include <boost/signals2.hpp>
#include <SDL2/SDL.h>

#include <nano_engine/engine.hpp>
#include <nano_engine/system.hpp>

namespace ne
{
class input_system : public system
{
public:
  input_system           ()
  {
    if (SDL_Init(SDL_INIT_EVENTS) != 0)
      std::cout << "Failed to initialize display system. SDL Error: " << SDL_GetError() << std::endl;
  }
  input_system           (const input_system&  that) = default;
  input_system           (      input_system&& temp) = default;
  virtual ~input_system  ()
  {
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
  }
  input_system& operator=(const input_system&  that) = default;
  input_system& operator=(      input_system&& temp) = default;

  bool get_key_state(unsigned char key ) const
  {
    assert(key < key_states_.size());
    return key_states_[key];
  }

  static void lock_mouse(bool lock)
  {
    if (SDL_SetRelativeMouseMode(SDL_bool(lock)) != 0)
      std::cout << "Unable to lock mouse. SDL Error: " << SDL_GetError() << std::endl;
  }

  boost::signals2::signal<void()>                   on_quit          ;
  boost::signals2::signal<void(char)>               on_key_pressed   ;
  boost::signals2::signal<void(char)>               on_key_released  ;
  boost::signals2::signal<void(unsigned, unsigned)> on_mouse_moved   ;
  boost::signals2::signal<void(unsigned)>           on_mouse_pressed ;
  boost::signals2::signal<void(unsigned)>           on_mouse_released;

protected:
  void initialize() override
  {
    on_quit.connect(std::bind(&engine::stop, engine_));
  }
  void tick    () override
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

    int  key_state_count;
    auto key_states     = SDL_GetKeyboardState(&key_state_count);
    key_states_.assign(key_states, key_states + key_state_count);
  }

  std::vector<bool> key_states_;
};
}

#endif
