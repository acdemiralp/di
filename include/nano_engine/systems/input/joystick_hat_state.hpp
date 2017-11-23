#ifndef NANO_ENGINE_SYSTEMS_INPUT_JOYSTICK_HAT_STATE_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_JOYSTICK_HAT_STATE_HPP_

#include <cstddef>

#include <SDL2/SDL_joystick.h>

namespace ne
{
enum class joystick_hat_state : std::uint8_t
{
  centered   = SDL_HAT_CENTERED ,
  up         = SDL_HAT_UP       ,
  right      = SDL_HAT_RIGHT    ,
  down       = SDL_HAT_DOWN     ,
  left       = SDL_HAT_LEFT     ,
  right_up   = SDL_HAT_RIGHTUP  ,
  right_down = SDL_HAT_RIGHTDOWN,
  left_up    = SDL_HAT_LEFTUP   ,
  left_down  = SDL_HAT_LEFTDOWN
};
}

#endif
