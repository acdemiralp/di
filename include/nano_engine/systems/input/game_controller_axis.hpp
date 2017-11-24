#ifndef NANO_ENGINE_SYSTEMS_INPUT_GAME_CONTROLLER_AXIS_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_GAME_CONTROLLER_AXIS_HPP_

#include <SDL2/SDL_gamecontroller.h>

namespace ne
{
enum class game_controller_axis
{
  invalid       = SDL_CONTROLLER_AXIS_INVALID     ,
  left_x        = SDL_CONTROLLER_AXIS_LEFTX       ,
  left_y        = SDL_CONTROLLER_AXIS_LEFTY       ,
  right_x       = SDL_CONTROLLER_AXIS_RIGHTX      ,
  right_y       = SDL_CONTROLLER_AXIS_RIGHTY      ,
  left_trigger  = SDL_CONTROLLER_AXIS_TRIGGERLEFT ,
  right_trigger = SDL_CONTROLLER_AXIS_TRIGGERRIGHT
};
}

#endif
