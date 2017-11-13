#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_MODE_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_DISPLAY_MODE_HPP_

#include <array>
#include <cstddef>

#include <SDL2/SDL_video.h>

namespace ne
{
struct display_mode
{
  explicit display_mode  (const SDL_DisplayMode& native) 
  : format(native.format), size({static_cast<std::size_t>(native.w), static_cast<std::size_t>(native.h)}), refresh_rate(static_cast<std::size_t>(native.refresh_rate))
  {

  }
  display_mode           ()                          = default;
  display_mode           (const display_mode&  that) = default;
  display_mode           (      display_mode&& temp) = default;
  ~display_mode          ()                          = default;
  display_mode& operator=(const display_mode&  that) = default;
  display_mode& operator=(      display_mode&& temp) = default;
    
  SDL_DisplayMode native() const
  {
    return {format, static_cast<int>(size[0]), static_cast<int>(size[1]), static_cast<int>(refresh_rate), nullptr};
  }

  std::uint32_t              format      ;
  std::array<std::size_t, 2> size        ;
  std::size_t                refresh_rate;
};
}

#endif
