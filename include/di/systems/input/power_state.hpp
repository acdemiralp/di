#ifndef DI_SYSTEMS_INPUT_POWER_STATE_HPP_
#define DI_SYSTEMS_INPUT_POWER_STATE_HPP_

#include <SDL2/SDL_power.h>

namespace di
{
enum class power_state
{
  unknown    = SDL_POWERSTATE_UNKNOWN   ,
  on_battery = SDL_POWERSTATE_ON_BATTERY,
  no_battery = SDL_POWERSTATE_NO_BATTERY,
  charging   = SDL_POWERSTATE_CHARGING  ,
  charged    = SDL_POWERSTATE_CHARGED
};
}

#endif
