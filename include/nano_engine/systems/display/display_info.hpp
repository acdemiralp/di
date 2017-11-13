#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_INFO_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_INFO_HPP_

#include <array>
#include <cstddef>
#include <vector>

#include <SDL2/SDL.h>

#include <nano_engine/systems/display/display_mode.hpp>

namespace ne
{
struct display_info
{
  display_mode closest_display_mode(const display_mode& mode) const
  {
    auto native_mode = mode.native();
    SDL_DisplayMode native_closest_mode;
    SDL_GetClosestDisplayMode(static_cast<int>(index), &native_mode, &native_closest_mode);
    return display_mode(native_closest_mode);
  }

  std::size_t                index               ;
  std::string                name                ;
  std::array<std::size_t, 2> size                ;
  std::array<std::size_t, 2> usable_size         ;
  std::array<float      , 3> dpi                 ; // width, height, diagonal
  std::vector<display_mode>  display_modes       ;
  display_mode               current_display_mode;
  display_mode               desktop_display_mode;
};
}

#endif
