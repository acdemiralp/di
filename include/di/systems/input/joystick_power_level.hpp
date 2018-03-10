#ifndef DI_SYSTEMS_INPUT_JOYSTICK_POWER_LEVEL_HPP_
#define DI_SYSTEMS_INPUT_JOYSTICK_POWER_LEVEL_HPP_

#include <SDL2/SDL_joystick.h>

namespace di
{
enum class joystick_power_level
{
  unknown = SDL_JOYSTICK_POWER_UNKNOWN,
  empty   = SDL_JOYSTICK_POWER_EMPTY  ,
  low     = SDL_JOYSTICK_POWER_LOW    ,
  medium  = SDL_JOYSTICK_POWER_MEDIUM ,
  full    = SDL_JOYSTICK_POWER_FULL   ,
  wired   = SDL_JOYSTICK_POWER_WIRED  ,
  maximum = SDL_JOYSTICK_POWER_MAX
};
}

#endif
