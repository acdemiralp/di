#ifndef DI_SYSTEMS_INPUT_GAME_CONTROLLER_BUTTON_HPP_
#define DI_SYSTEMS_INPUT_GAME_CONTROLLER_BUTTON_HPP_

#include <SDL2/SDL_gamecontroller.h>

namespace di
{
enum class game_controller_button
{
  invalid        = SDL_CONTROLLER_BUTTON_INVALID      ,
  a              = SDL_CONTROLLER_BUTTON_A            ,
  b              = SDL_CONTROLLER_BUTTON_B            ,
  x              = SDL_CONTROLLER_BUTTON_X            ,
  y              = SDL_CONTROLLER_BUTTON_Y            ,
  back           = SDL_CONTROLLER_BUTTON_BACK         ,
  guide          = SDL_CONTROLLER_BUTTON_GUIDE        ,
  start          = SDL_CONTROLLER_BUTTON_START        ,
  left_stick     = SDL_CONTROLLER_BUTTON_LEFTSTICK    ,
  right_stick    = SDL_CONTROLLER_BUTTON_RIGHTSTICK   ,
  left_shoulder  = SDL_CONTROLLER_BUTTON_LEFTSHOULDER ,
  right_shoulder = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
  up             = SDL_CONTROLLER_BUTTON_DPAD_UP      ,
  down           = SDL_CONTROLLER_BUTTON_DPAD_DOWN    ,
  left           = SDL_CONTROLLER_BUTTON_DPAD_LEFT    ,
  right          = SDL_CONTROLLER_BUTTON_DPAD_RIGHT
};
}

#endif
