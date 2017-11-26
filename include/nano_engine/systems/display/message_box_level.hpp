#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_MESSAGE_BOX_LEVEL_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_MESSAGE_BOX_LEVEL_HPP_

#include <SDL2/SDL_messagebox.h>

namespace ne
{
enum class message_box_level : unsigned
{
  error       = SDL_MESSAGEBOX_ERROR      ,
  warning     = SDL_MESSAGEBOX_WARNING    ,
  information = SDL_MESSAGEBOX_INFORMATION
};
}

#endif
