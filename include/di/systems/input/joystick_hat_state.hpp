#ifndef DI_SYSTEMS_INPUT_JOYSTICK_HAT_STATE_HPP_
#define DI_SYSTEMS_INPUT_JOYSTICK_HAT_STATE_HPP_

#include <cstdint>

#include <SDL2/SDL_joystick.h>

namespace di
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
