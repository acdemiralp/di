#ifndef NANO_ENGINE_SYSTEMS_INPUT_OS_CURSOR_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_OS_CURSOR_HPP_

#include <SDL2/SDL_mouse.h>

namespace ne
{
enum class os_cursor
{
  arrow              = SDL_SYSTEM_CURSOR_ARROW,
  ibeam              = SDL_SYSTEM_CURSOR_IBEAM,
  wait               = SDL_SYSTEM_CURSOR_WAIT,
  crosshair          = SDL_SYSTEM_CURSOR_CROSSHAIR,
  wait_arrow         = SDL_SYSTEM_CURSOR_WAITARROW,
  double_arrow_nw_se = SDL_SYSTEM_CURSOR_SIZENWSE,
  double_arrow_ne_sw = SDL_SYSTEM_CURSOR_SIZENESW,
  double_arrow_w_e   = SDL_SYSTEM_CURSOR_SIZEWE,
  double_arrow_n_s   = SDL_SYSTEM_CURSOR_SIZENS,
  four_arrows        = SDL_SYSTEM_CURSOR_SIZEALL,
  no                 = SDL_SYSTEM_CURSOR_NO,
  hand               = SDL_SYSTEM_CURSOR_HAND
};
}

#endif
