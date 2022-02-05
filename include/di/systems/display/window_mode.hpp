#ifndef DI_SYSTEMS_DISPLAY_WINDOW_MODE_HPP_
#define DI_SYSTEMS_DISPLAY_WINDOW_MODE_HPP_

#include <SDL2/SDL.h>

namespace di
{
enum class window_mode
{
  windowed            = 0,
  fullscreen          = SDL_WINDOW_FULLSCREEN,
  fullscreen_windowed = SDL_WINDOW_FULLSCREEN_DESKTOP
};
}

#endif
