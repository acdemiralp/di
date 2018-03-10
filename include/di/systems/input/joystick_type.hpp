#ifndef DI_SYSTEMS_INPUT_JOYSTICK_TYPE_HPP_
#define DI_SYSTEMS_INPUT_JOYSTICK_TYPE_HPP_

#include <SDL2/SDL_joystick.h>

namespace di
{
enum class joystick_type
{
  unknown         = SDL_JOYSTICK_TYPE_UNKNOWN       ,
  game_controller = SDL_JOYSTICK_TYPE_GAMECONTROLLER,
  wheel           = SDL_JOYSTICK_TYPE_WHEEL         ,
  arcade_stick    = SDL_JOYSTICK_TYPE_ARCADE_STICK  ,
  flight_stick    = SDL_JOYSTICK_TYPE_FLIGHT_STICK  ,
  dance_pad       = SDL_JOYSTICK_TYPE_DANCE_PAD     ,
  guitar          = SDL_JOYSTICK_TYPE_GUITAR        ,
  drum_kit        = SDL_JOYSTICK_TYPE_DRUM_KIT      ,
  arcade_pad      = SDL_JOYSTICK_TYPE_ARCADE_PAD    ,
  throttle        = SDL_JOYSTICK_TYPE_THROTTLE
};
}

#endif
