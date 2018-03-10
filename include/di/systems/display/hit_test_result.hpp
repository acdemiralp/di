#ifndef DI_SYSTEMS_DISPLAY_WINDOW_HIT_TEST_RESULT_HPP_
#define DI_SYSTEMS_DISPLAY_WINDOW_HIT_TEST_RESULT_HPP_

#include <SDL2/SDL_video.h>

namespace di
{
enum class hit_test_result
{
  normal              = SDL_HITTEST_NORMAL            ,
  draggable           = SDL_HITTEST_DRAGGABLE         ,
  resize_top_left     = SDL_HITTEST_RESIZE_TOPLEFT    ,
  resize_top          = SDL_HITTEST_RESIZE_TOP        ,
  resize_top_right    = SDL_HITTEST_RESIZE_TOPRIGHT   ,
  resize_right        = SDL_HITTEST_RESIZE_RIGHT      ,
  resize_bottom_right = SDL_HITTEST_RESIZE_BOTTOMRIGHT,
  resize_bottom       = SDL_HITTEST_RESIZE_BOTTOM     ,
  resize_bottom_left  = SDL_HITTEST_RESIZE_BOTTOMLEFT ,
  resize_left         = SDL_HITTEST_RESIZE_LEFT
};
}

#endif
