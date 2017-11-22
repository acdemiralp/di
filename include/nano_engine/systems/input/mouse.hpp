#ifndef NANO_ENGINE_SYSTEMS_INPUT_MOUSE_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_MOUSE_HPP_

#include <array>
#include <cstddef>
#include <vector>

#include <SDL2/SDL_mouse.h>

#include <nano_engine/systems/input/os_cursor.hpp>

namespace ne
{
namespace mouse
{
inline void                       set_visible          (bool visible )
{
  SDL_ShowCursor(static_cast<int>(visible));
}
inline void                       set_captured         (bool captured)
{
  SDL_CaptureMouse(static_cast<SDL_bool>(captured));
}

inline void                       set_relative         (bool relative)
{
  SDL_SetRelativeMouseMode(static_cast<SDL_bool>(relative));
}
inline bool                       relative             ()
{
  return static_cast<bool>(SDL_GetRelativeMouseMode());
}

inline void                       set_absolute_position(const std::array<std::size_t, 2>& position)
{
  SDL_WarpMouseGlobal(static_cast<int>(position[0]), static_cast<int>(position[1]));
}
inline std::array<std::size_t, 2> absolute_position    ()
{
  std::array<std::size_t, 2> position;
  SDL_GetGlobalMouseState(reinterpret_cast<int*>(&position[0]), reinterpret_cast<int*>(&position[1]));
  return position;
}

inline std::array<std::size_t, 2> relative_position    (bool delta = false)
{
  std::array<std::size_t, 2> position;
  delta ?
    SDL_GetRelativeMouseState(reinterpret_cast<int*>(&position[0]), reinterpret_cast<int*>(&position[1])): 
    SDL_GetMouseState        (reinterpret_cast<int*>(&position[0]), reinterpret_cast<int*>(&position[1]));
  return position;
}

inline bool                       is_button_pressed    (std::size_t button)
{
  return SDL_GetGlobalMouseState(nullptr, nullptr) & static_cast<Uint32>(button);
}

inline void                       redraw               ()
{
  SDL_SetCursor(nullptr);
}
inline void                       set_cursor_default   ()
{
  auto current_cursor  = SDL_GetCursor       ();
  if  (current_cursor != SDL_GetDefaultCursor())
    SDL_FreeCursor(current_cursor);
  SDL_SetCursor(SDL_GetDefaultCursor());
}
inline void                       set_cursor           (std::string pixels, std::array<std::size_t, 2> hotspot = {0, 0})
{
  /* Pixels have to form a square and each dimension must be a multiple of 8. Example:
    "00000000000000000000000000000000"
    "00000000000000000000000000000000"
    "00000000000000000000000000000000"
    "00000000000000000000000000000000"
    "00001111111100000000111111110000"
    "00001111111100000000111111110000"
    "00001111111100000000111111110000"
    "00001111111100000000111111110000"
    "00001111111100000000111111110000"
    "00001111111100000000111111110000"
    "00001111111100000000111111110000"
    "00001111111100000000111111110000"
    "00000000000000000000000000000000"
    "00000000000000000000000000000000"
    "00000000000001111110000000000000"
    "00000000000001111110000000000000"
    "00000000000001111110000000000000"
    "00000000000001111110000000000000"
    "00000000000000000000000000000000"
    "00000000000000000000000000000000"
    "00000111111111111111111111100000"
    "00000111111111111111111111100000"
    "00000111111111111111111111100000"
    "00000111111111111111111111100000"
    "00000111111111111111111111100000"
    "00000111111111111111111111100000"
    "00000111111111111111111111100000"
    "00000111111111111111111111100000"
    "00000111111111111111111111100000"
    "00000111111111111111111111100000"
    "00000000000000000000000000000000"
    "00000000000000000000000000000000" */
  auto squared_size = static_cast<std::size_t>(sqrt(pixels.size()));
  std::vector<std::uint8_t> data(pixels.size() / 8);
  std::vector<std::uint8_t> mask(pixels.size() / 8);
  for(auto i = 0, j = -1; i < pixels.size(); ++i)
  {
    if(i % 8)
    {
      data[j] <<= 1;
      mask[j] <<= 1;
    }
    else
    {
      ++j;
      data[j] = 0;
      mask[j] = 0;
    }

    if      (pixels[i] == '1')
    {
      data[j] |= 0x01;
      mask[j] |= 0x01;
    }
    else if (pixels[i] == '0')
    {
      mask[j] |= 0x01;
    }
  }
  auto current_cursor  = SDL_GetCursor       ();
  if  (current_cursor != SDL_GetDefaultCursor())
    SDL_FreeCursor(current_cursor);
  SDL_SetCursor(SDL_CreateCursor(data.data(), mask.data(), static_cast<int>(squared_size), static_cast<int>(squared_size), static_cast<int>(hotspot[0]), static_cast<int>(hotspot[1])));
}
inline void                       set_cursor           (os_cursor os_cursor)
{
  auto current_cursor  = SDL_GetCursor       ();
  if  (current_cursor != SDL_GetDefaultCursor())
    SDL_FreeCursor(current_cursor);
  SDL_SetCursor(SDL_CreateSystemCursor(static_cast<SDL_SystemCursor>(os_cursor)));
}
}
}

#endif
