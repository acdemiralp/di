#ifndef DI_SYSTEMS_DISPLAY_WINDOW_FLAGS_HPP_
#define DI_SYSTEMS_DISPLAY_WINDOW_FLAGS_HPP_

#include <cstdint>

#include <SDL2/SDL_video.h>

#include <di/utility/bitset_enum.hpp>

namespace di
{
enum class window_flags : std::uint32_t
{
  none               = 0u                      ,
  allow_high_dpi     = SDL_WINDOW_ALLOW_HIGHDPI,
  always_on_top      = SDL_WINDOW_ALWAYS_ON_TOP,
  skip_taskbar       = SDL_WINDOW_SKIP_TASKBAR ,
  utility_window     = SDL_WINDOW_UTILITY      ,
  tooltip            = SDL_WINDOW_TOOLTIP      ,
  popup_menu         = SDL_WINDOW_POPUP_MENU
};
}

template<>
struct is_bitset_enum<di::window_flags>
{
  static const bool enable = true;
};

#endif
