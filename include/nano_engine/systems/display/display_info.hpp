#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_INFO_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_INFO_HPP_

#include <array>
#include <cstddef>
#include <string>
#include <vector>

#include <SDL2/SDL_video.h>

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

inline std::vector<display_info> displays()
{
  std::vector<display_info> displays;
  for(auto i = 0; i < SDL_GetNumVideoDisplays(); ++i)
  {
    const auto name = std::string(SDL_GetDisplayName(i));

    SDL_Rect bounds, usable_bounds;
    SDL_GetDisplayBounds      (i, &bounds       );
    SDL_GetDisplayUsableBounds(i, &usable_bounds);

    std::array<float, 3> dpi;
    SDL_GetDisplayDPI(i, &dpi[2], &dpi[0], &dpi[1]);
      
    std::vector<display_mode> display_modes;
    for(auto j = 0; j < SDL_GetNumDisplayModes(i); ++j)
    {
      SDL_DisplayMode native_display_mode;
      SDL_GetDisplayMode(i, j, &native_display_mode);
      display_modes.emplace_back(native_display_mode);
    }

    SDL_DisplayMode native_current_display_mode, native_desktop_display_mode;
    SDL_GetCurrentDisplayMode(i, &native_current_display_mode);
    SDL_GetDesktopDisplayMode(i, &native_desktop_display_mode);

    displays.push_back({
      static_cast<std::size_t>(i),
      name,
      {static_cast<std::size_t>(bounds       .w), static_cast<std::size_t>(bounds       .h)},
      {static_cast<std::size_t>(usable_bounds.w), static_cast<std::size_t>(usable_bounds.h)},
      dpi,
      display_modes,
      display_mode(native_current_display_mode),
      display_mode(native_desktop_display_mode)
    });
  }
  return displays;
}
}

#endif
