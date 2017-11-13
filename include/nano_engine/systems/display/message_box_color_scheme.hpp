#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_MESSAGE_BOX_COLOR_SCHEME_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_MESSAGE_BOX_COLOR_SCHEME_HPP_

#include <array>
#include <cstddef>

#include <SDL2/SDL_messagebox.h>

namespace ne
{
struct message_box_color_scheme
{
  SDL_MessageBoxColorScheme native() const
  {
    return
    {{
      {background       [0], background       [1], background       [2]},
      {text             [0], text             [1], text             [2]},
      {button_border    [0], button_border    [1], button_border    [2]},
      {button_background[0], button_background[1], button_background[2]},
      {button_selected  [0], button_selected  [1], button_selected  [2]}
    }};
  }

  std::array<std::uint8_t, 3> background        = {200, 200, 200};
  std::array<std::uint8_t, 3> text              = {  0,   0,   0};
  std::array<std::uint8_t, 3> button_border     = {175, 175, 175};
  std::array<std::uint8_t, 3> button_background = {100, 100, 100};
  std::array<std::uint8_t, 3> button_selected   = {150, 150, 150};
};
}

#endif
